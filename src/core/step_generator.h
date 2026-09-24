#pragma once
#include <string>
#include <vector>

namespace mogg {

class StepGenerator {
public:
    StepGenerator() = default;

    void addStep(const std::string& stepDescription);
    void clearSteps();

    [[nodiscard]] std::vector<std::string> getSteps() const;
    [[nodiscard]] std::string format() const;

private:
    std::vector<std::string> m_steps;
};

} // namespace mogg