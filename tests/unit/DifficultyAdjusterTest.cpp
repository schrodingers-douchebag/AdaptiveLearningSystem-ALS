#include <gtest/gtest.h>
#include "algorithms/DifficultyAdjuster.hpp"

using als::DifficultyAdjuster;

TEST(DifficultyAdjuster, KeepsLevelWithTooFewAnswers) {
    EXPECT_EQ(DifficultyAdjuster::adjust(2, 1.0, 2, 2).newLevel, 2);
}
TEST(DifficultyAdjuster, ExcellentAccuracyMovesUp) {
    EXPECT_EQ(DifficultyAdjuster::adjust(2, 0.95, 5, 5).newLevel, 3);
}
TEST(DifficultyAdjuster, GoodAccuracyNeedsAStreak) {
    EXPECT_EQ(DifficultyAdjuster::adjust(3, 0.8, 5, 1).newLevel, 3);
    EXPECT_EQ(DifficultyAdjuster::adjust(3, 0.8, 5, 2).newLevel, 4);
}
TEST(DifficultyAdjuster, MixedResultsHold) {
    EXPECT_EQ(DifficultyAdjuster::adjust(3, 0.5, 5, 0).newLevel, 3);
}
TEST(DifficultyAdjuster, LowAccuracyMovesDown) {
    EXPECT_EQ(DifficultyAdjuster::adjust(3, 0.2, 5, 0).newLevel, 2);
}
TEST(DifficultyAdjuster, NeverLeavesTheRange) {
    EXPECT_EQ(DifficultyAdjuster::adjust(5, 1.0, 5, 5).newLevel, 5);
    EXPECT_EQ(DifficultyAdjuster::adjust(1, 0.0, 5, 0).newLevel, 1);
}
