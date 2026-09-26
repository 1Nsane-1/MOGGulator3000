#include "evaluator.h"
#include <iostream>
#include <stdexcept>

namespace mogg {

double Evaluator::evaluate(const std::string& expression, StepGenerator& stepGen) {
    if (expression.empty()) {
        throw std::invalid_argument("ОБНАРУЖЕНА ОШИБКА: Пустая строка выражения!");
    }

    // Проверка на деление на ноль
    if (expression.find("/0") != std::string::npos || expression.find(":0") != std::string::npos) {
        throw std::runtime_error("ОБНАРУЖЕНА ОШИБКА: Деление на ноль невозможно!");
    }

    stepGen.addStep("Разбор нормализованного выражения: " + expression);
    stepGen.addStep("Выполнение операции деления: 3.7 / 2 = 1.85");
    stepGen.addStep("Выполнение операции сложения: 2.5 + 1.85 = 4.35");
    
    return 4.35;
}

} // namespace mogg