#include <iostream>
#include <chrono>
#include "core/math_expression.h"
#include "core/evaluator.h"
#include "core/step_generator.h"
#include "db/history_entry.h"
#include "db/history_repository.h"

// Переключатель отладочного режима (1 = ВКЛ, 0 = ВЫКЛ)
#define DEBUG_MODE 1

int main() {
    #if DEBUG_MODE
        std::cout << "[DEBUG LOG] Запуск приложения в режиме отладки (Linux x86_64)" << std::endl;
    #endif

    auto startTime = std::chrono::high_resolution_clock::now();

    // 1. Нормализация выражения
    mogg::MathExpression expr(" 2.5 + 3,7 : 2 ");
    
    #if DEBUG_MODE
        std::cout << "[DEBUG LOG] Исходная строка: \"" << expr.getRaw() << "\"" << std::endl;
        std::cout << "[DEBUG LOG] Нормализованная строка: \"" << expr.getNormalized() << "\"" << std::endl;
    #endif

    // 2. Вычисление и шаги
    mogg::StepGenerator stepGen;
    mogg::Evaluator evaluator;
    double result = evaluator.evaluate(expr.getNormalized(), stepGen);

    // 3. Сохранение истории
    mogg::HistoryEntry entry(1, expr.getNormalized(), std::to_string(result), "2026-09-26");
    mogg::HistoryRepository repository;
    repository.save(entry);

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    std::cout << "\n=== MOGGulator3000 (Linux Build) ===" << std::endl;
    std::cout << "Выражение: " << expr.getNormalized() << std::endl;
    std::cout << "Результат:  " << result << std::endl;
    std::cout << "Шаги решения:\n" << stepGen.format();
    std::cout << "Время выполнения: " << duration << " мкс" << std::endl;

    return 0;
}