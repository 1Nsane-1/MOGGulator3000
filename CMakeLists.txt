cmake_minimum_required(VERSION 3.14)
project(MOGGulator3000 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

set(SOURCES
    src/main.cpp
    src/core/evaluator.cpp
    src/core/math_expression.cpp
    src/core/step_generator.cpp
    src/db/history_entry.cpp
    src/db/history_repository.cpp
)

add_executable(MOGGulator3000 ${SOURCES})

# Подключение модульных тестов
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()