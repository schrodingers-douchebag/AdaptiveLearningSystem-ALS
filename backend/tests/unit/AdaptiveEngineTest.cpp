#include <gtest/gtest.h>
#include "algorithms/AdaptiveEngine.hpp"

using namespace als;

static std::vector<Question> bank() {
    std::vector<Question> v;
    int64_t id = 1;
    for (const char* topic : {"Arrays", "Graphs"})
        for (int d = 1; d <= 5; ++d) { Question q; q.id = id++; q.topic = topic; q.difficulty = d; v.push_back(q); }
    return v;
}

TEST(KnowledgeTracker, CorrectRaisesAndWrongLowersMastery) {
    EXPECT_GT(KnowledgeTracker::update(0.5, true, 3), 0.5);
    EXPECT_LT(KnowledgeTracker::update(0.5, false, 3), 0.5);
}
TEST(KnowledgeTracker, HardCorrectAnswersCountMore) {
    EXPECT_GT(KnowledgeTracker::update(0.5, true, 5), KnowledgeTracker::update(0.5, true, 1));
}
TEST(KnowledgeTracker, StaysInsideZeroToOne) {
    double m = 0.99;
    for (int i = 0; i < 50; ++i) m = KnowledgeTracker::update(m, true, 5);
    EXPECT_LE(m, 1.0);
    for (int i = 0; i < 200; ++i) m = KnowledgeTracker::update(m, false, 1);
    EXPECT_GE(m, 0.0);
}
TEST(AdaptiveEngine, StreakOfCorrectAnswersRaisesLevel) {
    AdaptiveEngine engine;
    StudentState s;
    s.level = 2;
    auto b = bank();
    int64_t qid = 1;
    EngineResult r;
    for (int i = 0; i < 4; ++i) {
        AnswerEvent e{qid, "Arrays", 2, true, 10};
        r = engine.processAnswer(s, e, b);
        // feed the result back in like the service layer would
        s.level = r.newLevel;
        s.topics["Arrays"] = r.updatedTopic;
        s.recentAttempts.insert(s.recentAttempts.begin(), AttemptRecord{qid, "Arrays", 2, true, 10});
        s.answeredCounts[qid]++;
    }
    EXPECT_GT(r.newLevel, 2);
    EXPECT_GT(r.updatedTopic.mastery, 0.7);
}
TEST(AdaptiveEngine, WrongAnswersLowerLevelAndTargetTheWeakTopic) {
    AdaptiveEngine engine;
    StudentState s;
    s.level = 3;
    auto b = bank();
    EngineResult r;
    for (int i = 0; i < 4; ++i) {
        r = engine.processAnswer(s, AnswerEvent{static_cast<int64_t>(i + 1), "Graphs", 3, false, 30}, b);
        s.level = r.newLevel;
        s.topics["Graphs"] = r.updatedTopic;
        s.recentAttempts.insert(s.recentAttempts.begin(), AttemptRecord{static_cast<int64_t>(i + 1), "Graphs", 3, false, 30});
    }
    EXPECT_LT(r.newLevel, 3);
    ASSERT_TRUE(r.next.has_value());
    EXPECT_EQ(r.next->topic, "Graphs");
}
TEST(AdaptiveEngine, RecommendNextHandlesEmptyBank) {
    AdaptiveEngine engine;
    EXPECT_FALSE(engine.recommendNext(StudentState{}, {}).has_value());
}
