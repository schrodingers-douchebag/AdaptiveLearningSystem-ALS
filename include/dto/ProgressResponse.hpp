#pragma once
#include <nlohmann/json.hpp>
#include "algorithms/PerformanceAnalyzer.hpp"
#include "models/LearningProgress.hpp"

namespace als {
inline nlohmann::json toJson(const LearningProgress& p) {
    return {{"topic", p.topic}, {"mastery_score", p.masteryScore}, {"total_attempts", p.totalAttempts},
            {"correct_attempts", p.correctAttempts},
            {"accuracy", p.totalAttempts ? static_cast<double>(p.correctAttempts) / p.totalAttempts : 0.0},
            {"updated_at", p.updatedAt}};
}
inline nlohmann::json toJson(const PerformanceSummary& s) {
    nlohmann::json topics = nlohmann::json::array();
    for (const auto& t : s.topics)
        topics.push_back({{"topic", t.topic}, {"attempts", t.attempts}, {"correct", t.correct}, {"accuracy", t.accuracy}});
    return {{"total_attempts", s.totalAttempts}, {"correct", s.correct}, {"incorrect", s.incorrect},
            {"accuracy", s.accuracy}, {"average_time_seconds", s.averageTime},
            {"recent_accuracy", s.recentAccuracy}, {"trend", s.trend}, {"trend_label", s.trendLabel},
            {"consistency", s.consistency}, {"correct_streak", s.correctStreak}, {"topics", topics}};
}
}  // namespace als
