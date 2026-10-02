#include <iostream>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>

#include "core/evaluator.h"
#include "core/math_expression.h"
#include "core/step_generator.h"
#include "db/history_repository.h"
#include "core/exceptions.h"

using namespace mogg;
namespace fs = std::filesystem;

void run_test(const std::string& test_name, void (*test_func)()) {
    try {
        test_func();
        std::cout << "[PASSED] " << test_name << "\n";
    } catch (const std::exception& e) {
        std::cerr << "[FAILED] " << test_name << ": " << e.what() << "\n";
        exit(1);
    }
}

// === Тесты Evaluator ===
void test_evaluator_basic_sum() {
    Evaluator eval;
    StepGenerator stepGen;
    assert(std::abs(eval.evaluate("2 + 2", stepGen) - 4.0) < Evaluator::COMPARISON_EPSILON);
}

void test_evaluator_operator_priority() {
    Evaluator eval;
    StepGenerator stepGen;
    assert(std::abs(eval.evaluate("2 + 3 * 4", stepGen) - 14.0) < Evaluator::COMPARISON_EPSILON);
}

void test_evaluator_parentheses() {
    Evaluator eval;
    StepGenerator stepGen;
    assert(std::abs(eval.evaluate("(2 + 3) * 4", stepGen) - 20.0) < Evaluator::COMPARISON_EPSILON);
}

void test_evaluator_unary_minus_with_spaces() {
    Evaluator eval;
    StepGenerator stepGen;
    assert(std::abs(eval.evaluate(" - 5 + 10", stepGen) - 5.0) < Evaluator::COMPARISON_EPSILON);
}

void test_evaluator_division_by_zero_throws() {
    Evaluator eval;
    StepGenerator stepGen;
    bool thrown = false;
    try {
        eval.evaluate("10 / 0", stepGen);
    } catch (const MathError&) {
        thrown = true;
    }
    assert(thrown);
}

void test_evaluator_invalid_syntax_throws() {
    Evaluator eval;
    StepGenerator stepGen;
    bool thrown = false;
    try {
        eval.evaluate("2 + * 3", stepGen);
    } catch (const ValidationError&) {
        thrown = true;
    }
    assert(thrown);
}

// === Тесты MathExpression ===
void test_expression_normalization_commas() {
    MathExpression expr("2,5 + 3,5");
    assert(expr.getNormalized() == "2.5 + 3.5");
}

void test_expression_normalization_colon() {
    MathExpression expr("12:4");
    assert(expr.getNormalized() == "12/4");
}

// === Тесты StepGenerator ===
void test_step_generator_creation() {
    Evaluator eval;
    StepGenerator generator;
    eval.evaluate("2 + 3 * 4", generator);
    assert(!generator.getSteps().empty());
}

// === Тесты HistoryRepository ===
void test_history_auto_directory_creation() {
    fs::remove_all("data_test");
    HistoryRepository repo;
    
    HistoryEntry entry{1, "5 + 5", "10", "2026-10-02"};
    repo.save(entry, "data_test/history.txt");
    
    assert(fs::exists("data_test/history.txt"));
    fs::remove_all("data_test");
}

void test_history_save_and_read() {
    fs::remove_all("data_test");
    HistoryRepository repo;
    
    HistoryEntry entry{2, "10 * 10", "100", "2026-10-02"};
    repo.save(entry, "data_test/history.txt");
    
    std::ifstream file("data_test/history.txt");
    std::string line;
    std::getline(file, line);
    assert(!line.empty());
    
    file.close();
    fs::remove_all("data_test");
}

void test_empty_input_validation() {
    Evaluator eval;
    StepGenerator stepGen;
    bool thrown = false;
    try {
        eval.evaluate("   ", stepGen);
    } catch (const ValidationError&) {
        thrown = true;
    }
    assert(thrown);
}

int main() {
    std::cout << "=== Запуск Unit-тестов MOGGulator3000 ===\n";
    
    run_test("TC-UNIT-01: Базовое сложение", test_evaluator_basic_sum);
    run_test("TC-UNIT-02: Приоритет операций", test_evaluator_operator_priority);
    run_test("TC-UNIT-03: Обработка скобок", test_evaluator_parentheses);
    run_test("TC-UNIT-04: Унарный минус с пробелами", test_evaluator_unary_minus_with_spaces);
    run_test("TC-UNIT-05: Исключение при делении на ноль", test_evaluator_division_by_zero_throws);
    run_test("TC-UNIT-06: Исключение при ошибке синтаксиса", test_evaluator_invalid_syntax_throws);
    run_test("TC-UNIT-07: Нормализация запятых", test_expression_normalization_commas);
    run_test("TC-UNIT-08: Нормализация двоеточий", test_expression_normalization_colon);
    run_test("TC-UNIT-09: Генерация шагов вычисления", test_step_generator_creation);
    run_test("TC-UNIT-10: Автосоздание папки истории", test_history_auto_directory_creation);
    run_test("TC-UNIT-11: Запись и сохранение истории", test_history_save_and_read);
    run_test("TC-UNIT-12: Валидация пустого ввода", test_empty_input_validation);

    std::cout << "=== Все 12 unit-тестов успешно пройдены (GREEN) ===\n";
    return 0;
}