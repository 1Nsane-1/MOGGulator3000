#include "db/history_repository.h"
#include "core/exceptions.h"
#include <fstream>

namespace mogg {

void HistoryRepository::save(const HistoryEntry& entry, const std::string& filepath) {
    std::ofstream outFile(filepath, std::ios::app);
    if (!outFile.is_open()) {
        throw StorageError("Ошибка ввода-вывода: Не удалось открыть файл хранилища по пути: " + filepath);
    }
    
    outFile << entry.getNormalizedExpr() << " = " << entry.getResult() << " [" << entry.getTimestamp() << "]\n";
    outFile.close();

    m_storage.push_back(entry);
}

} // namespace mogg