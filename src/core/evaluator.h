#pragma once
#include <string>
#include "step_generator.h"

namespace mogg {

class Evaluator {
public:
    Evaluator() = default;
    
    // Вычисление нормализованного выражения с генерацией шагов
    double evaluate(const std::string& expression, StepGenerator& stepGen);
};

} // namespace mogg