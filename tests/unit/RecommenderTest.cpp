#include <gtest/gtest.h>
#include "algorithms/QuestionRecommender.hpp"

using namespace als;

static Question q(int64_t id, const char* topic, int diff) {
    Question x;
    x.id = id; x.topic = topic; x.difficulty = diff;
    return x;
}

TEST(Recommender, PrefersWeakTopicAtMatchingDifficulty) {
    RecommenderInput in;
    in.targetLevel = 3;
    in.topicMastery = {{"Weak", 0.2}, {"Strong", 0.9}};
    auto best = QuestionRecommender::recommend({q(1, "Strong", 3), q(2, "Weak", 3)}, in);
    ASSERT_EQ(best.size(), 1u);
    EXPECT_EQ(best[0].questionId, 2);
}
TEST(Recommender, PenalisesDifficultyMismatch) {
    RecommenderInput in;
    in.targetLevel = 2;
    in.topicMastery = {{"T", 0.5}};
    auto best = QuestionRecommender::recommend({q(1, "T", 5), q(2, "T", 2)}, in);
    EXPECT_EQ(best[0].questionId, 2);
}
TEST(Recommender, SkipsRecentlyAskedQuestions) {
    RecommenderInput in;
    in.targetLevel = 2;
    in.recentQuestionIds = {1};
    auto best = QuestionRecommender::recommend({q(1, "T", 2), q(2, "T", 4)}, in);
    EXPECT_EQ(best[0].questionId, 2);
}
TEST(Recommender, FallsBackWhenEverythingIsRecent) {
    RecommenderInput in;
    in.recentQuestionIds = {1};
    EXPECT_EQ(QuestionRecommender::recommend({q(1, "T", 2)}, in).size(), 1u);
}
TEST(Recommender, ScoreFactorsStayBetweenZeroAndOne) {
    RecommenderInput in;
    in.targetLevel = 4;
    in.answeredCounts = {{1, 3}};
    auto s = QuestionRecommender::score(q(1, "T", 1), in);
    for (double f : {s.weakTopic, s.difficultyFit, s.recentRelevance, s.novelty}) {
        EXPECT_GE(f, 0.0);
        EXPECT_LE(f, 1.0);
    }
    EXPECT_GE(s.score, 0.0);
    EXPECT_LE(s.score, 1.0);
}
