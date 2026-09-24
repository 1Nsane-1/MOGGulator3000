#pragma once
#include <string>

namespace mogg {

class MathExpression {
public:
    MathExpression() = default;
    explicit MathExpression(std::string rawStr);

    void setRaw(const std::string& rawStr);
    [[nodiscard]] std::string getRaw() const;
    [[nodiscard]] std::string getNormalized() const;

    // Нормализация строки: удаление пробелов, замена двоеточий/запятых
    std::string normalize();

private:
    std::string m_raw;
    std::string m_normalized;
};

} // namespace mogg