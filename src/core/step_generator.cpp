#include "step_generator.h"
#include <sstream>

namespace mogg {

void StepGenerator::addStep(const std::string& stepDescription) {
    if (!stepDescription.empty()) {
        m_steps.push_back(stepDescription);
    }
}

void StepGenerator::clearSteps() {
    m_steps.clear();
}

std::vector<std::string> StepGenerator::getSteps() const {
    return m_steps;
}

std::string StepGenerator::format() const {
    if (m_steps.empty()) {
        return "Шаги решения отсутствуют.";
    }

    std::ostringstream formattedOutput;
    for (size_t i = 0; i < m_steps.size(); ++i) {
        formattedOutput << (i + 1) << ". " << m_steps[i] << "\n";
    }
    return formattedOutput.str();
}

} // namespace mogg