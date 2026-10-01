include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  unit_tests
  parser_test.cpp
  ../src/core/evaluator.cpp
  ../src/core/math_expression.cpp
  ../src/core/step_generator.cpp
  ../src/db/history_repository.cpp
  ../src/db/history_entry.cpp
)

target_include_directories(unit_tests PRIVATE ../src)
target_link_libraries(unit_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
enable_testing()

add_executable(unit_tests
    tests/unit_tests.cpp
    src/core/evaluator.cpp
    src/core/math_expression.cpp
    src/core/step_generator.cpp
    src/db/history_repository.cpp
)

target_include_directories(unit_tests PRIVATE src)
add_test(NAME MOGGulatorUnitTests COMMAND unit_tests)