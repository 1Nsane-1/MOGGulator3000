#pragma once
#include <string>

namespace mogg {

class HistoryEntry {
public:
    HistoryEntry() = default;
    HistoryEntry(int id, std::string expression, std::string result, std::string date);

    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getExpression() const;
    [[nodiscard]] std::string getResult() const;
    [[nodiscard]] std::string getDate() const;

    [[nodiscard]] std::string toJson() const;

private:
    int m_id{0};
    std::string m_expression;
    std::string m_result;
    std::string m_date;
};

} // namespace mogg