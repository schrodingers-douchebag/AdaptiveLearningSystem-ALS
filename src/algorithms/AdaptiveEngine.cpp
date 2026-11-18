#include "algorithms/AdaptiveEngine.hpp"

#include <algorithm>
#include "algorithms/AlgorithmConfig.hpp"
#include "algorithms/DifficultyAdjuster.hpp"

namespace als {

RecommenderInput AdaptiveEngine::buildInput(const StudentState& state, int level) {
    RecommenderInput in;
    in.targetLevel = level;
    in.answeredCounts = state.answeredCounts;
    for (const auto& [topic, tm] : state.topics) in.topicMastery[topic] = tm.mastery;

    // recent error rate overall + per topic (look at the most recent two windows)
    const size_t limit = std::min<size_t>(state.recentAttempts.size(), cfg::kRecentWindow * 2);
    std::unordered_map<std::string, std::pair<int, int>> perTopic;  // topic -> {wrong, total}
    int wrong = 0;
    for (size_t i = 0; i < limit; ++i) {
        const auto& r = state.recentAttempts[i];
        auto& p = perTopic[r.topic];
        p.second++;
        if (!r.correct) { p.first++; wrong++; }
    }
    in.overallRecentError = limit ? static_cast<double>(wrong) / limit : 0.0;
    for (const auto& [topic, p] : perTopic) in.topicRecentError[topic] = static_cast<double>(p.first) / p.second;

    for (size_t i = 0; i < state.recentAttempts.size() && i < static_cast<size_t>(cfg::kRecentQuestionBlock); ++i)
        in.recentQuestionIds.push_back(state.recentAttempts[i].questionId);
    return in;
}

std::optional<ScoredQuestion> AdaptiveEngine::recommendNext(const StudentState& state,
                                                            const std::vector<Question>& candidates) const {
    auto best = QuestionRecommender::recommend(candidates, buildInput(state, state.level), 1);
    if (best.empty()) return std::nullopt;
    return best.front();
}

EngineResult AdaptiveEngine::processAnswer(StudentState state, const AnswerEvent& e,
                                           const std::vector<Question>& candidates) const {
    EngineResult out;
    out.topic = e.topic;
    out.previousLevel = state.level;

    // 1-2: record the attempt and update this topic's mastery
    state.recentAttempts.insert(state.recentAttempts.begin(),
                                AttemptRecord{e.questionId, e.topic, e.difficulty, e.correct, e.timeTaken});
    state.answeredCounts[e.questionId]++;
    TopicMastery& tm = state.topics[e.topic];  // new topics start at kInitialMastery
    tm.mastery = KnowledgeTracker::update(tm.mastery, e.correct, e.difficulty);
    tm.attempts++;
    out.updatedTopic = tm;

    // 3: analyse recent performance
    out.summary = PerformanceAnalyzer::analyze(state.recentAttempts);

    // 4: adjust difficulty from the rolling window
    const int inWindow = static_cast<int>(std::min<size_t>(state.recentAttempts.size(), cfg::kRecentWindow));
    LevelChange change = DifficultyAdjuster::adjust(state.level, out.summary.recentAccuracy, inWindow,
                                                    out.summary.correctStreak);
    state.level = change.newLevel;
    out.newLevel = change.newLevel;
    out.levelReason = change.reason;

    // 5-6: rank questions with the new level and pick the best
    out.next = recommendNext(state, candidates);
    return out;
}

}  // namespace als
