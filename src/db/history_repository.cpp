#include "history_repository.h"
#include "exceptions.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void HistoryRepository::save(const std::string& expression, double result) {
    std::string filepath = "data/history.txt";
    
    try {
        fs::path path(filepath);
        if (path.has_parent_path()) {
            fs::create_directories(path.parent_path());
        }
    } catch (const std::exception& e) {
        throw StorageError("Не удалось создать директорию для истории: " + std::string(e.what()));
    }

    std::ofstream outFile(filepath, std::ios::app);
    if (!outFile.is_open()) {
        throw StorageError("Не удалось открыть файл истории для записи");
    }

    outFile << expression << " = " << result << "\n";
}