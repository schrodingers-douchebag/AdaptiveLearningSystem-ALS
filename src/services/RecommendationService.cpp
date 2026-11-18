#include "services/RecommendationService.hpp"

#include "algorithms/AlgorithmConfig.hpp"

namespace als {

StudentState RecommendationService::buildState(int64_t userId) {
    StudentState s;
    s.level = progress_.ensureProfile(userId, cfg::kStartLevel).currentLevel;
    for (const auto& p : progress_.all(userId)) s.topics[p.topic] = TopicMastery{p.masteryScore, p.totalAttempts};
    s.recentAttempts = answers_.recentAttempts(userId, cfg::kHistoryWindow);
    s.answeredCounts = answers_.answeredCounts(userId);
    return s;
}

std::optional<ScoredQuestion> RecommendationService::next(int64_t userId, bool persist) {
    auto pick = engine_.recommendNext(buildState(userId), questions_.listAll());
    if (pick && persist) recs_.save(userId, pick->questionId, pick->score);
    return pick;
}

}  // namespace als
