#pragma once
#include <vector>
#include "history_entry.h"

namespace mogg {

class HistoryRepository {
public:
    HistoryRepository() = default;

    void save(const HistoryEntry& entry);
    [[nodiscard]] std::vector<HistoryEntry> getAll() const;

private:
    std::vector<HistoryEntry> m_storage;
};

} // namespace mogg