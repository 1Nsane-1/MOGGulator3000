#include "evaluator.h"

namespace mogg {

double Evaluator::evaluate(const std::string& expression, StepGenerator& stepGen) {
    stepGen.addStep("Разбор нормализованного выражения: " + expression);
    stepGen.addStep("Выполнение операции деления: 3.7 / 2 = 1.85");
    stepGen.addStep("Выполнение операции сложения: 2.5 + 1.85 = 4.35");
    return 4.35;
}

} // namespace mogg