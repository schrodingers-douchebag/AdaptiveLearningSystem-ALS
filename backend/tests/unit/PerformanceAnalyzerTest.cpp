#include <gtest/gtest.h>
#include "algorithms/PerformanceAnalyzer.hpp"

using namespace als;

static AttemptRecord rec(const char* topic, bool ok, int t = 10) { return {1, topic, 2, ok, t}; }

TEST(PerformanceAnalyzer, EmptyHistoryIsSafe) {
    auto s = PerformanceAnalyzer::analyze({});
    EXPECT_EQ(s.totalAttempts, 0);
    EXPECT_EQ(s.trendLabel, "not enough data");
}
TEST(PerformanceAnalyzer, CountsAccuracyTimeAndStreak) {
    // newest first: right, right, wrong, right
    auto s = PerformanceAnalyzer::analyze({rec("A", true, 10), rec("A", true, 20), rec("B", false, 30), rec("B", true, 40)});
    EXPECT_EQ(s.correct, 3);
    EXPECT_EQ(s.incorrect, 1);
    EXPECT_DOUBLE_EQ(s.accuracy, 0.75);
    EXPECT_DOUBLE_EQ(s.averageTime, 25.0);
    EXPECT_EQ(s.correctStreak, 2);
}
TEST(PerformanceAnalyzer, TopicsSortedWeakestFirst) {
    auto s = PerformanceAnalyzer::analyze({rec("Strong", true), rec("Strong", true), rec("Weak", false), rec("Weak", true), rec("Weak", false)});
    ASSERT_EQ(s.topics.size(), 2u);
    EXPECT_EQ(s.topics[0].topic, "Weak");
}
TEST(PerformanceAnalyzer, DetectsImprovingTrend) {
    std::vector<AttemptRecord> a;
    for (int i = 0; i < 5; ++i) a.push_back(rec("A", true));   // recent window: all right
    for (int i = 0; i < 5; ++i) a.push_back(rec("A", false));  // earlier window: all wrong
    auto s = PerformanceAnalyzer::analyze(a);
    EXPECT_EQ(s.trendLabel, "improving");
    EXPECT_NEAR(s.trend, 1.0, 1e-9);
}
