cmake_minimum_required(VERSION 3.16)
project(MOGGulator3000 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем каталог src для поиска заголовочных файлов (.h)
include_directories(src)

# Список всех исходных файлов C++
set(SOURCES
    src/main.cpp
    src/core/math_expression.cpp
    src/core/step_generator.cpp
    src/db/history_entry.cpp
)

# Исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Поддиректория для юнит-тестов
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
endif()