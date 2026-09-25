#include <iostream>
#include <windows.h> // Добавлено для Windows UTF-8
#include "core/math_expression.h"
#include "core/step_generator.h"
#include "db/history_entry.h"

int main() {
    // Включаем поддержку UTF-8 в консоли Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "=== MOGGulator3000 Test Run ===" << std::endl << std::endl;

    // 1. Проверка модуля MathExpression
    mogg::MathExpression expr(" 2.5 + 3,7 : 2 ");
    std::cout << "[MathExpression Test]" << std::endl;
    std::cout << "Raw input:        \"" << expr.getRaw() << "\"" << std::endl;
    std::cout << "Normalized output: \"" << expr.getNormalized() << "\"" << std::endl << std::endl;

    // 2. Проверка модуля StepGenerator
    mogg::StepGenerator steps;
    steps.addStep("Деление: 3.7 / 2 = 1.85");
    steps.addStep("Сложение: 2.5 + 1.85 = 4.35");

    std::cout << "[StepGenerator Test]" << std::endl;
    std::cout << steps.format() << std::endl;

    // 3. Проверка модуля HistoryEntry
    mogg::HistoryEntry entry(1, expr.getNormalized(), "4.35", "2026-09-24");
    std::cout << "[HistoryEntry JSON Test]" << std::endl;
    std::cout << entry.toJson() << std::endl;

    return 0;
} 