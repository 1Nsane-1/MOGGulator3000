#include <gtest/gtest.h>
#include <chrono>
#include "core/evaluator.h"
#include "core/math_expression.h"
#include "core/step_generator.h"
#include "core/exceptions.h"
#include "db/history_repository.h"

// 1. Базовая арифметика
TEST(EvaluatorTest, BasicArithmetic) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_DOUBLE_EQ(eval.evaluate("10+15-5", gen), 20.0);
    EXPECT_DOUBLE_EQ(eval.evaluate("12*4/2", gen), 24.0);
}

// 2. Приоритеты и вложенные скобки
TEST(EvaluatorTest, NestedBracketsAndPrecedence) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_DOUBLE_EQ(eval.evaluate("(2+3)*4", gen), 20.0);
    EXPECT_DOUBLE_EQ(eval.evaluate("((10-2)/2)+1", gen), 5.0);
}

// 3. Отрицательные числа (унарный минус)
TEST(EvaluatorTest, NegativeNumbers) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_DOUBLE_EQ(eval.evaluate("-5+10", gen), 5.0);
}

// 4. Прямое и косвенное деление на ноль
TEST(EvaluatorTest, DivisionByZero) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("10/0", gen), mogg::MathError);
    EXPECT_THROW(eval.evaluate("15/(5-5)", gen), mogg::MathError);
}

// 5. Обработка пустой строки и одних пробелов
TEST(EvaluatorTest, EmptyAndWhitespaceInput) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("", gen), mogg::ValidationError);
    EXPECT_THROW(eval.evaluate("   ", gen), mogg::ValidationError);
}

// 6. Недопустимые символы и синтаксические ошибки
TEST(EvaluatorTest, InvalidSyntax) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("2.5+abc-5", gen), mogg::ValidationError);
    EXPECT_THROW(eval.evaluate("2++2", gen), mogg::ValidationError);
}

// 7. Проверка нормализации строки (MathExpression)
TEST(MathExpressionTest, NormalizationCommasAndColons) {
    mogg::MathExpression expr1("2,5 + 3,7");
    EXPECT_EQ(expr1.getNormalized(), "2.5+3.7");

    mogg::MathExpression expr2("12 : 4");
    EXPECT_EQ(expr2.getNormalized(), "12/4");
}

// 8. Генерация шагов решений
TEST(StepGeneratorTest, StepTracking) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    eval.evaluate("2.5+3.7/2", gen);
    EXPECT_FALSE(gen.getSteps().empty());
}

// 9. Сохранение истории вычислений
TEST(HistoryRepositoryTest, SaveToFile) {
    mogg::HistoryRepository repo;
    mogg::HistoryEntry entry(1, "5+5", "10", "2026-09-28");
    EXPECT_NO_THROW(repo.save(entry, "data/test_history.txt"));
}

// 10. Воспроизводимый тест производительности (Benchmark c std::chrono)
TEST(PerformanceTest, BenchmarkCalculationSeries) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        eval.evaluate("((10+20)*3-5)/(2+3)", gen);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    // Порог: 1000 сложных вычислений должны занимать менее 100 мс
    EXPECT_LT(duration_ms, 100);
}