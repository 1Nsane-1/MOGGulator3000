#include "math_expression.h"
#include <algorithm>

namespace mogg {

MathExpression::MathExpression(std::string rawStr)
    : m_raw(std::move(rawStr)) {
    normalize();
}

void MathExpression::setRaw(const std::string& rawStr) {
    m_raw = rawStr;
    normalize();
}

std::string MathExpression::getRaw() const {
    return m_raw;
}

std::string MathExpression::getNormalized() const {
    return m_normalized;
}

std::string MathExpression::normalize() {
    m_normalized = m_raw;
    
    // Удаление пробельных символов
    m_normalized.erase(
        std::remove_if(m_normalized.begin(), m_normalized.end(), ::isspace),
        m_normalized.end()
    );

    // Замена запятой на точку для поддержки вещественных чисел[cite: 4]
    std::replace(m_normalized.begin(), m_normalized.end(), ',', '.');

    // Замена двоеточия на знак деления
    std::replace(m_normalized.begin(), m_normalized.end(), ':', '/');

    return m_normalized;
}

} // namespace mogg