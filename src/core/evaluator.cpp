#include "evaluator.h"
#include "exceptions.h"
#include <cmath>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <sstream>

namespace mogg {

std::vector<std::string> Evaluator::tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    bool expectOperand = true; // Флаг для определения унарного минуса

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (std::isspace(c)) continue;

        // Обработка чисел (включая дроби с точкой или запятой)
        if (std::isdigit(c) || c == '.' || c == ',') {
            std::string num;
            while (i < expression.length() && (std::isdigit(expression[i]) || expression[i] == '.' || expression[i] == ',')) {
                if (expression[i] == ',') num += '.';
                else num += expression[i];
                ++i;
            }
            --i;
            tokens.push_back(num);
            expectOperand = false;
        } 
        // Обработка унарного минуса
        else if (c == '-' && expectOperand) {
            std::string num = "-";
            ++i;
            while (i < expression.length() && std::isspace(expression[i])) ++i;
            while (i < expression.length() && (std::isdigit(expression[i]) || expression[i] == '.' || expression[i] == ',')) {
                if (expression[i] == ',') num += '.';
                else num += expression[i];
                ++i;
            }
            --i;
            tokens.push_back(num);
            expectOperand = false;
        } 
        // Обработка скобок и операторов
        else if (c == '(') {
            tokens.push_back("(");
            expectOperand = true;
        } else if (c == ')') {
            tokens.push_back(")");
            expectOperand = false;
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == ':') {
            std::string op(1, c == ':' ? '/' : c); // Заменяем двоеточие на слеш
            tokens.push_back(op);
            expectOperand = true;
        } else {
            throw ValidationError("Недопустимый символ в выражении");
        }
    }
    return tokens;
}

std::vector<std::string> Evaluator::convertToRPN(const std::vector<std::string>& tokens) {
    std::vector<std::string> rpn;
    std::stack<std::string> ops;
    
    // Лямбда для определения приоритета операций
    auto precedence = [](const std::string& op) {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        return 0;
    };

    for (const auto& token : tokens) {
        if (std::isdigit(token.back())) { // Если это число
            rpn.push_back(token);
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                rpn.push_back(ops.top());
                ops.pop();
            }
            if (ops.empty()) throw ValidationError("Несогласованные скобки");
            ops.pop();
        } else { // Если это оператор
            while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
                rpn.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
    }

    // Выталкиваем оставшиеся операторы
    while (!ops.empty()) {
        if (ops.top() == "(") throw ValidationError("Несогласованные скобки");
        rpn.push_back(ops.top());
        ops.pop();
    }
    
    return rpn;
}

double Evaluator::calculateRPN(const std::vector<std::string>& rpnTokens, StepGenerator& stepGen) {
    std::stack<double> stack;

    for (const auto& token : rpnTokens) {
        if (std::isdigit(token.back())) {
            stack.push(std::stod(token));
        } else {
            if (stack.size() < 2) throw ValidationError("Синтаксическая ошибка: неверный формат операторов");
            
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            double res = 0;

            if (token == "+") res = a + b;
            else if (token == "-") res = a - b;
            else if (token == "*") res = a * b;
            else if (token == "/") {
                if (std::abs(b) < COMPARISON_EPSILON) {
                    throw MathError("Деление на ноль");
                }
                res = a / b;
            }

            stack.push(res);
            
            // Формируем строку и передаем ее в генератор шагов
            std::ostringstream stepStream;
            stepStream << a << " " << token << " " << b << " = " << res;
            stepGen.addStep(stepStream.str());
        }
    }

    if (stack.size() != 1) throw ValidationError("Синтаксическая ошибка в выражении");
    return stack.top();
}

double Evaluator::evaluate(const std::string& expression, StepGenerator& stepGen) {
    // Валидация пустого ввода
    if (expression.empty() || expression.find_first_not_of(' ') == std::string::npos) {
        throw ValidationError("Пустое выражение");
    }

    std::vector<std::string> tokens = tokenize(expression);
    std::vector<std::string> rpn = convertToRPN(tokens);
    return calculateRPN(rpn, stepGen);
}

} // namespace mogg