#include "history_entry.h"
#include <sstream>

namespace mogg {

HistoryEntry::HistoryEntry(int id, std::string expression, std::string result, std::string date)
    : m_id(id), m_expression(std::move(expression)), m_result(std::move(result)), m_date(std::move(date)) {}

int HistoryEntry::getId() const { return m_id; }
std::string HistoryEntry::getExpression() const { return m_expression; }
std::string HistoryEntry::getResult() const { return m_result; }
std::string HistoryEntry::getDate() const { return m_date; }

std::string HistoryEntry::toJson() const {
    std::ostringstream jsonStream;
    jsonStream << "{\n"
               << "  \"id\": " << m_id << ",\n"
               << "  \"expression\": \"" << m_expression << "\",\n"
               << "  \"result\": \"" << m_result << "\",\n"
               << "  \"date\": \"" << m_date << "\"\n"
               << "}";
    return jsonStream.str();
}

} // namespace mogg