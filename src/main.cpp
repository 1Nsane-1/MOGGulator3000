#include <iostream>
#include <vector>
#include "core/math_expression.h"
#include "core/evaluator.h"
#include "core/step_generator.h"
#include "core/exceptions.h"
#include "db/history_entry.h"
#include "db/history_repository.h"

void processExpression(const std::string& rawInput, const std::string& savePath = "data/history.txt") {
    std::cout << "\n-----------------------------------" << std::endl;
    std::cout << "Входная строка: \"" << rawInput << "\"" << std::endl;

    try {
        mogg::MathExpression expr(rawInput);
        mogg::StepGenerator stepGen;
        mogg::Evaluator evaluator;

        double result = evaluator.evaluate(expr.getNormalized(), stepGen);

        mogg::HistoryEntry entry(1, expr.getNormalized(), std::to_string(result), "2026-09-28");
        mogg::HistoryRepository repository;
        
        // Попытка сохранения в файл
        repository.save(entry, savePath);

        std::cout << " Успешный расчет и сохранение! Результат: " << result << std::endl;
        std::cout << "Шаги решения:\n" << stepGen.format();

    } catch (const mogg::ValidationError& e) {
        std::cout << "⚠️ [ПРЕДУПРЕЖДЕНИЕ ВАЛИДАЦИИ] (" << e.getErrorCode() << "): " << e.what() << std::endl;
    } catch (const mogg::MathError& e) {
        std::cout << "❌ [МАТЕМАТИЧЕСКАЯ ОШИБКА] (" << e.getErrorCode() << "): " << e.what() << std::endl;
    } catch (const mogg::StorageError& e) {
        std::cout << "💾 [ОШИБКА ХРАНИЛИЩА] (" << e.getErrorCode() << "): " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "🆘 [СИСТЕМНАЯ ОШИБКА]: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Тестирование устойчивости MOGGulator3000 к ошибкам ===" << std::endl;

    // 1. Стандартные тесты
    processExpression("2.5 + 3,7 : 2");   // Корректно
    processExpression("10 / 0");          // MathError
    processExpression("2.5 + abc - 5");   // ValidationError
    processExpression("");                 // ValidationError

    // 2. Тест исключения StorageError (передаем несуществующий путь)
    std::cout << "\n=== Тестирование StorageError ===";
    processExpression("5 + 5", "/invalid_dir_path/history.txt");

    std::cout << "\nВсе исключения успешно обработаны. Приложение продолжает работу!" << std::endl;
    return 0;
}