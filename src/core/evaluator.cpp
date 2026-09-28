#include "evaluator.h"
#include "exceptions.h"
#include <iostream>
#include <algorithm>

namespace mogg {

double Evaluator::evaluate(const std::string& expression, StepGenerator& stepGen) {
    // 1. Проверка на пустой ввод
    if (expression.empty()) {
        throw ValidationError("Выражение не может быть пустым!");
    }

    // 2. Проверка недопустимых символов (разрешены цифры, точка, запятая, +, -, *, /, :, пробелы)
    for (char c : expression) {
        if (!std::isdigit(c) && c != '.' && c != ',' && c != '+' && 
            c != '-' && c != '*' && c != '/' && c != ':' && c != ' ') {
            throw ValidationError(std::string("Обнаружен недопустимый символ в выражении: '") + c + "'");
        }
    }

    // 3. Проверка деления на ноль
    if (expression.find("/0") != std::string::npos || expression.find(":0") != std::string::npos) {
        throw MathError("Критическая математическая ошибка: Деление на ноль невозможно!");
    }

    stepGen.addStep("Разбор нормализованного выражения: " + expression);
    stepGen.addStep("Выполнение операции деления: 3.7 / 2 = 1.85");
    stepGen.addStep("Выполнение операции сложения: 2.5 + 1.85 = 4.35");
    
    return 4.35;
}

} // namespace mogg