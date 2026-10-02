#include "history_repository.h"
#include "../core/exceptions.h"
#include <fstream>
#include <filesystem>

namespace mogg {

void HistoryRepository::save(const HistoryEntry& historyEntry, const std::string& filepath) {
    namespace fs = std::filesystem;
    
    try {
        fs::path path(filepath);
        if (path.has_parent_path() && !fs::exists(path.parent_path())) {
            fs::create_directories(path.parent_path());
        }
    } catch (const std::exception& e) {
        throw StorageError("Не удалось создать директорию для истории: " + std::string(e.what()));
    }

    std::ofstream file(filepath, std::ios::app);
    if (!file.is_open()) {
        throw StorageError("Не удалось открыть файл истории для записи");
    }

    file << "[" << historyEntry.getDate() << "] " 
         << historyEntry.getExpression() << " = " 
         << historyEntry.getResult() << "\n";
}

} // namespace mogg