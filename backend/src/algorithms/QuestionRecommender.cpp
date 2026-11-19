#include "algorithms/QuestionRecommender.hpp"

#include <algorithm>
#include <cmath>
#include "algorithms/AlgorithmConfig.hpp"
#include "algorithms/KnowledgeTracker.hpp"

namespace als {

ScoredQuestion QuestionRecommender::score(const Question& q, const RecommenderInput& in) {
    ScoredQuestion s;
    s.questionId = q.id;
    s.topic = q.topic;
    s.difficulty = q.difficulty;

    // M: weaker topic -> higher value. Unknown topics count as "medium" (0.5).
    auto m = in.topicMastery.find(q.topic);
    s.weakTopic = 1.0 - (m != in.topicMastery.end() ? m->second : cfg::kInitialMastery);

    // D: how close the question difficulty is to the student's level.
    const double range = cfg::kMaxLevel - cfg::kMinLevel;
    s.difficultyFit = 1.0 - std::abs(q.difficulty - in.targetLevel) / range;

    // R: topics where the student made recent mistakes get more weight.
    auto e = in.topicRecentError.find(q.topic);
    s.recentRelevance = e != in.topicRecentError.end() ? e->second : in.overallRecentError;

    // N: never-seen questions are best; repeats fade out as 1/(1+times).
    auto c = in.answeredCounts.find(q.id);
    s.novelty = c == in.answeredCounts.end() ? 1.0 : 1.0 / (1.0 + c->second);

    s.score = cfg::kWeightWeakTopic * s.weakTopic + cfg::kWeightDifficulty * s.difficultyFit +
              cfg::kWeightRecent * s.recentRelevance + cfg::kWeightNovelty * s.novelty;
    return s;
}

std::vector<ScoredQuestion> QuestionRecommender::recommend(const std::vector<Question>& candidates,
                                                           const RecommenderInput& in, size_t topK) {
    // Eligibility: skip the last few questions (when there is anything else to ask).
    std::vector<int64_t> blocked;
    for (size_t i = 0; i < in.recentQuestionIds.size() && i < static_cast<size_t>(cfg::kRecentQuestionBlock); ++i)
        blocked.push_back(in.recentQuestionIds[i]);
    auto isBlocked = [&](int64_t id) { return std::find(blocked.begin(), blocked.end(), id) != blocked.end(); };

    std::vector<ScoredQuestion> scored;
    for (const auto& q : candidates)
        if (!isBlocked(q.id)) scored.push_back(score(q, in));
    if (scored.empty())  // tiny question bank: fall back to everything
        for (const auto& q : candidates) scored.push_back(score(q, in));

    std::sort(scored.begin(), scored.end(), [](const ScoredQuestion& a, const ScoredQuestion& b) {
        if (std::abs(a.score - b.score) > 1e-9) return a.score > b.score;
        return a.questionId < b.questionId;  // deterministic tie-break
    });
    if (scored.size() > topK) scored.resize(topK);
    return scored;
}

}  // namespace als
