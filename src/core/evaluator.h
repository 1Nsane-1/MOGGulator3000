#pragma once
#include <string>
#include <vector>
#include "step_generator.h"

namespace mogg {

class Evaluator {
public:
    static constexpr double COMPARISON_EPSILON = 1e-6;

    Evaluator() = default;
    double evaluate(const std::string& expression, StepGenerator& stepGen);

private:
    // NC-05: Разбиение монолитной функции на отдельные этапы
    std::vector<std::string> tokenize(const std::string& expression);
    std::vector<std::string> convertToRPN(const std::vector<std::string>& tokens);
    double calculateRPN(const std::vector<std::string>& rpnTokens, StepGenerator& stepGen);
};

} // namespace mogg