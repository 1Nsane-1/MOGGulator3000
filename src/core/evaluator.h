#pragma once
#include <string>
#include "step_generator.h"

namespace mogg {

class Evaluator {
public:
    static constexpr double COMPARISON_EPSILON = 1e-6;

    Evaluator() = default;
    double evaluate(const std::string& expression, StepGenerator& stepGen);
};

} // namespace mogg