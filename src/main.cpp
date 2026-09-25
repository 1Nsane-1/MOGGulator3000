#include <iostream>
#include <windows.h>
#include "core/math_expression.h"
#include "core/evaluator.h"
#include "core/step_generator.h"
#include "db/history_entry.h"
#include "db/history_repository.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "=== MOGGulator3000: Интеграционный запуск ===" << std::endl << std::endl;

    // 1. Модуль MathExpression (Ввод и нормализация)
    mogg::MathExpression expr(" 2.5 + 3,7 : 2 ");
    std::cout << "[1. MathExpression] Входная строка: \"" << expr.getRaw() << "\"" << std::endl;
    std::cout << "[1. MathExpression] После нормализации: \"" << expr.getNormalized() << "\"" << std::endl << std::endl;

    // 2. Модуль StepGenerator & 3. Модуль Evaluator
    mogg::StepGenerator stepGen;
    mogg::Evaluator evaluator;
    double result = evaluator.evaluate(expr.getNormalized(), stepGen);

    std::cout << "[2. Evaluator] Вычисленный результат: " << result << std::endl << std::endl;
    std::cout << "[3. StepGenerator] Сгенерированные шаги решения:" << std::endl;
    std::cout << stepGen.format() << std::endl;

    // 4. Модуль HistoryEntry & 5. Модуль HistoryRepository
    mogg::HistoryEntry entry(1, expr.getNormalized(), "4.35", "2026-09-25");
    mogg::HistoryRepository repository;
    repository.save(entry);

    std::cout << "[4 & 5. HistoryRepository] Запись сохранена в БД:" << std::endl;
    for (const auto& item : repository.getAll()) {
        std::cout << item.toJson() << std::endl;
    }

    std::cout << "\n=== Интеграция прошла успешно! ===" << std::endl;
    return 0;
}