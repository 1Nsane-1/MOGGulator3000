#include <iostream>
#include <vector>
#include "core/math_expression.h"
#include "core/evaluator.h"
#include "core/step_generator.h"
#include "core/exceptions.h"
#include "db/history_entry.h"
#include "db/history_repository.h"

void processExpression(const std::string& rawInput) {
    std::cout << "\n-----------------------------------" << std::endl;
    std::cout << "Входная строка: \"" << rawInput << "\"" << std::endl;

    try {
        mogg::MathExpression expr(rawInput);
        mogg::StepGenerator stepGen;
        mogg::Evaluator evaluator;

        double result = evaluator.evaluate(expr.getNormalized(), stepGen);

        mogg::HistoryEntry entry(1, expr.getNormalized(), std::to_string(result), "2026-09-27");
        mogg::HistoryRepository repository;
        repository.save(entry);

        std::cout << " Успешный расчет! Результат: " << result << std::endl;
        std::cout << "Шаги решения:\n" << stepGen.format();

    } catch (const mogg::ValidationError& e) {
        std::cout << "⚠️ [ПРЕДУПРЕЖДЕНИЕ ВАЛИДАЦИИ] (Код: " << e.getErrorCode() << "): " << e.what() << std::endl;
    } catch (const mogg::MathError& e) {
        std::cout << "❌ [МАТЕМАТИЧЕСКАЯ ОШИБКА] (Код: " << e.getErrorCode() << "): " << e.what() << std::endl;
    } catch (const mogg::StorageError& e) {
        std::cout << "💾 [ОШИБКА ХРАНИЛИЩА] (Код: " << e.getErrorCode() << "): " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "🆘 [НЕИЗВЕСТНАЯ СИСТЕМНАЯ ОШИБКА]: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Тестирование устойчивости MOGGulator3000 к ошибкам ===" << std::endl;

    // Набор тестов (корректные и некорректные)
    std::vector<std::string> testInputs = {
        "2.5 + 3,7 : 2",   // Корректное выражение
        "10 / 0",          // Деление на ноль
        "2.5 + abc - 5",   // Недопустимые символы
        ""                 // Пустая строка
    };

    for (const auto& input : testInputs) {
        processExpression(input);
    }

    std::cout << "\nВсе исключения успешно обработаны. Приложение продолжает работу!" << std::endl;
    return 0;
}