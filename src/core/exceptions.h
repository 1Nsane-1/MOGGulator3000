#ifndef MOGG_EXCEPTIONS_H
#define MOGG_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace mogg {

// Базовый класс для всех исключений MOGGулятора
class MoggException : public std::runtime_error {
private:
    std::string errorCode_;
public:
    MoggException(const std::string& message, const std::string& errorCode)
        : std::runtime_error(message), errorCode_(errorCode) {}

    std::string getErrorCode() const { return errorCode_; }
};

// 1. Ошибка валидации и синтаксиса выражения
class ValidationError : public MoggException {
public:
    explicit ValidationError(const std::string& message)
        : MoggException(message, "VALIDATION_ERROR") {}
};

// 2. Математическая ошибка (деление на ноль)
class MathError : public MoggException {
public:
    explicit MathError(const std::string& message)
        : MoggException(message, "MATH_ERROR") {}
};

// 3. Ошибка сохранения истории в файл/БД
class StorageError : public MoggException {
public:
    explicit StorageError(const std::string& message)
        : MoggException(message, "STORAGE_ERROR") {}
};

} // namespace mogg

#endif // MOGG_EXCEPTIONS_H