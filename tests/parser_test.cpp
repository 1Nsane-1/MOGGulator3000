#include <gtest/gtest.h>
#include <chrono>
#include <fstream>
#include "core/evaluator.h"
#include "core/math_expression.h"
#include "core/step_generator.h"
#include "core/exceptions.h"
#include "db/history_repository.h"

// --- TS-01: Базовая арифметика ---
TEST(EvaluatorTest, BasicArithmetic) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_DOUBLE_EQ(eval.evaluate("10+15-5", gen), 20.0);
    EXPECT_DOUBLE_EQ(eval.evaluate("12*4/2", gen), 24.0);
}

// --- TS-02: Приоритеты и вложенные скобки ---
TEST(EvaluatorTest, NestedBracketsAndPrecedence) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_DOUBLE_EQ(eval.evaluate("(2+3)*4", gen), 20.0);
    EXPECT_DOUBLE_EQ(eval.evaluate("((10-2)/2)+1", gen), 5.0);
}

// --- TS-03: Нормализация строк ---
TEST(MathExpressionTest, NormalizationCommasAndColons) {
    mogg::MathExpression expr1("2,5 + 3,7");
    EXPECT_EQ(expr1.getNormalized(), "2.5+3.7");

    mogg::MathExpression expr2("12 : 4");
    EXPECT_EQ(expr2.getNormalized(), "12/4");
}

// --- TS-04: Генерация шагов (Кейс 1 и Кейс 2) ---
TEST(StepGeneratorTest, StepTrackingAndOrder) {
    mogg::Evaluator eval;
    
    // Кейс 1: Наличие шагов (TC-06)
    mogg::StepGenerator gen1;
    eval.evaluate("2.5+3.7/2", gen1);
    EXPECT_FALSE(gen1.getSteps().empty());

    // Кейс 2: Порядок выполнения в сложном выражении (TC-25)
    mogg::StepGenerator gen2;
    eval.evaluate("(2+3)*4", gen2);
    auto steps = gen2.getSteps();
    ASSERT_GE(steps.size(), 2u);
    // Первый шаг должен быть сложением в скобках
    EXPECT_NE(steps[0].find("+"), std::string::npos);
}

// --- TS-05: Валидация недопустимых символов ---
TEST(EvaluatorTest, InvalidSyntax) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("2.5+abc-5", gen), mogg::ValidationError);
    EXPECT_THROW(eval.evaluate("2++2", gen), mogg::ValidationError);
}

// --- TS-06: Обработка пустого ввода ---
TEST(EvaluatorTest, EmptyAndWhitespaceInput) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("", gen), mogg::ValidationError);
    EXPECT_THROW(eval.evaluate("   ", gen), mogg::ValidationError);
}

// --- TS-07: Деление на ноль ---
TEST(EvaluatorTest, DivisionByZero) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("10/0", gen), mogg::MathError);
    EXPECT_THROW(eval.evaluate("15/(5-5)", gen), mogg::MathError);
}

// --- TS-08: Запись истории ---
TEST(HistoryRepositoryTest, SaveAndAppend) {
    mogg::HistoryRepository repo;
    mogg::HistoryEntry entry(1, "5+5", "10", "2026-09-28");
    EXPECT_NO_THROW(repo.save(entry, "data/test_history.txt"));
}

// --- TS-09: Ошибки ввода-вывода (Кейс 1 и Кейс 2) ---
TEST(HistoryRepositoryTest, StorageErrors) {
    mogg::HistoryRepository repo;
    mogg::HistoryEntry entry(1, "2+2", "4", "2026-09-28");
    
    // TC-14: Запись в несуществующую директорию
    EXPECT_THROW(repo.save(entry, "/non_existent_folder/file.txt"), mogg::StorageError);

    // TC-26: Запись в файл, открытый только для чтения (или защищенный путь)
    EXPECT_THROW(repo.save(entry, "/proc/invalid_store.txt"), mogg::StorageError);
}

// --- TS-10: Устойчивость цикла (Кейс 1 и Кейс 2) ---
TEST(SystemStabilityTest, FaultTolerance) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;

    // TC-15: Обычный восстанавливаемый сбой
    EXPECT_THROW(eval.evaluate("10/0", gen), mogg::MathError);
    EXPECT_NO_THROW(eval.evaluate("5+5", gen));

    // TC-27: Каскад сменяющих друг друга ошибок разного типа с итоговым успешным вычислением
    EXPECT_THROW(eval.evaluate("", gen), mogg::ValidationError);
    EXPECT_THROW(eval.evaluate("abc", gen), mogg::ValidationError);
    EXPECT_THROW(eval.evaluate("10/0", gen), mogg::MathError);
    EXPECT_DOUBLE_EQ(eval.evaluate("100-50", gen), 50.0);
}

// --- TS-11 & TS-12: Автоматизированные Воспроизводимые Performance-тесты ---
TEST(PerformanceBenchmark, TC21_SingleShortExpression) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    auto start = std::chrono::high_resolution_clock::now();
    eval.evaluate("2+2", gen);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    EXPECT_LT(duration, 1000); // Меньше 1 мс
}

TEST(PerformanceBenchmark, TC22_ComplexExpression) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    auto start = std::chrono::high_resolution_clock::now();
    eval.evaluate("((10+20)*3-5)/(2+3)", gen);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    EXPECT_LT(duration, 5000); // Меньше 5 мс
}

TEST(PerformanceBenchmark, TC23_ThousandCalculationsSeries) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        eval.evaluate("12*4/2", gen);
    }
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
    EXPECT_LT(duration_ms, 50); // Меньше 50 мс
}

TEST(PerformanceBenchmark, TC24_HundredStorageWrites) {
    mogg::HistoryRepository repo;
    mogg::HistoryEntry entry(1, "10+10", "20", "2026-09-28");
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        repo.save(entry, "data/benchmark_history.txt");
    }
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
    EXPECT_LT(duration_ms, 100); // Меньше 100 мс
}