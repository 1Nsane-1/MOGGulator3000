#ifndef HISTORY_REPOSITORY_H
#define HISTORY_REPOSITORY_H

#include "history_entry.h"
#include "core/exceptions.h"
#include <vector>
#include <string>

namespace mogg {

class HistoryRepository {
private:
    std::vector<HistoryEntry> m_storage;
public:
    // По умолчанию сохраняем в data/history.txt
    void save(const HistoryEntry& entry, const std::string& filepath = "data/history.txt");
};

} // namespace mogg

#endif