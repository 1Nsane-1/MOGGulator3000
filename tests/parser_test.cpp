#include <gtest/gtest.h>
#include "core/evaluator.h"
#include "core/math_expression.h"
#include "core/step_generator.h"
#include "core/exceptions.h"
#include "db/history_repository.h"

TEST(EvaluatorTest, BasicArithmetic) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_DOUBLE_EQ(eval.evaluate("10+15-5", gen), 20.0);
    EXPECT_DOUBLE_EQ(eval.evaluate("12*4/2", gen), 24.0);
}

TEST(EvaluatorTest, DivisionByZero) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("10/0", gen), mogg::MathError);
}

TEST(EvaluatorTest, InvalidCharacters) {
    mogg::Evaluator eval;
    mogg::StepGenerator gen;
    EXPECT_THROW(eval.evaluate("2.5+abc-5", gen), mogg::ValidationError);
}

TEST(HistoryRepositoryTest, SaveToFile) {
    mogg::HistoryRepository repo;
    mogg::HistoryEntry entry(1, "5+5", "10", "2026-09-28");
    EXPECT_NO_THROW(repo.save(entry, "data/test_history.txt"));
}