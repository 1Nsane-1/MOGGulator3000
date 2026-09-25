#include "history_repository.h"

namespace mogg {

void HistoryRepository::save(const HistoryEntry& entry) {
    m_storage.push_back(entry);
}

std::vector<HistoryEntry> HistoryRepository::getAll() const {
    return m_storage;
}

} // namespace mogg