cmake_minimum_required(VERSION 3.14)
project(MOGGulator3000 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматически собираем все исходники из src, исключая главный файл main.cpp
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")

# Обычная сборка приложения
add_executable(MOGGulator3000 src/main.cpp ${SRC_FILES})
target_include_directories(MOGGulator3000 PRIVATE src src/core src/db)

# Настройка unit-тестов для CTest / CI
enable_testing()

add_executable(unit_tests
    tests/unit_tests.cpp
    ${SRC_FILES}
)

target_include_directories(unit_tests PRIVATE src src/core src/db)
add_test(NAME MOGGulatorUnitTests COMMAND unit_tests)