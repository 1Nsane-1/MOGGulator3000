#pragma once
#include "history_entry.h"
#include <string>
#include <vector>

namespace mogg {

/// @brief Репозиторий для управления локальным файловым хранилищем истории вычислений.
class HistoryRepository {
public:
    static constexpr const char* DEFAULT_HISTORY_PATH = "data/history.txt";

    HistoryRepository() = default;

    /// @brief Сохраняет запись истории в текстовый файл.
    /// @param entry Объект записи истории.
    /// @param filepath Путь к файлу истории.
    /// @throws StorageError Если не удалось создать директорию или открыть файл.
    void save(const HistoryEntry& entry, const std::string& filepath = DEFAULT_HISTORY_PATH);
};

} // namespace mogg