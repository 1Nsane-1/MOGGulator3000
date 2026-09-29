#include "core/evaluator.h"
#include "core/exceptions.h"
#include <stack>
#include <string>
#include <cctype>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace mogg {

static int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

static double applyOp(double a, double b, char op, StepGenerator& stepGen) {
    double res = 0;
    if (op == '+') res = a + b;
    else if (op == '-') res = a - b;
    else if (op == '*') res = a * b;
    else if (op == '/') {
        if (std::abs(b) < 1e-9) {
            throw MathError("Деление на ноль невозможно!");
        }
        res = a / b;
    }
    std::ostringstream ss;
    ss << "Выполнение операции: " << a << " " << op << " " << b << " = " << res;
    stepGen.addStep(ss.str());
    return res;
}

double Evaluator::evaluate(const std::string& expression, StepGenerator& stepGen) {
    std::string expr = expression;
    expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

    if (expr.empty()) {
        throw ValidationError("Строка выражения пуста!");
    }

    for (char c : expr) {
        if (!std::isdigit(c) && c != '.' && c != '+' && c != '-' && c != '*' && c != '/' && c != '(' && c != ')') {
            throw ValidationError(std::string("Недопустимый символ в выражении: ") + c);
        }
    }

    std::stack<double> values;
    std::stack<char> ops;

    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == '(') {
            ops.push(expr[i]);
        } else if (std::isdigit(expr[i]) || expr[i] == '.') {
            std::string numStr;
            while (i < expr.length() && (std::isdigit(expr[i]) || expr[i] == '.')) {
                numStr += expr[i];
                i++;
            }
            i--;
            values.push(std::stod(numStr));
        } else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                if (values.size() < 2) throw ValidationError("Некорректный синтаксис выражения");
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op, stepGen));
            }
            if (!ops.empty()) ops.pop();
            else throw ValidationError("Несбалансированные скобки");
        } else {
            if (expr[i] == '-' && (i == 0 || expr[i-1] == '(')) {
                values.push(0);
            }
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                if (values.size() < 2) throw ValidationError("Некорректный синтаксис выражения");
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op, stepGen));
            }
            ops.push(expr[i]);
        }
    }

    while (!ops.empty()) {
        if (ops.top() == '(') throw ValidationError("Несбалансированные скобки");
        if (values.size() < 2) throw ValidationError("Некорректный синтаксис выражения");
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op, stepGen));
    }

    if (values.size() != 1) {
        throw ValidationError("Некорректный синтаксис выражения");
    }

    return values.top();
}

} // namespace mogg