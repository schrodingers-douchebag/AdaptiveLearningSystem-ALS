#include "services/AnalyticsService.hpp"

#include <algorithm>
#include "algorithms/AlgorithmConfig.hpp"
#include "algorithms/PerformanceAnalyzer.hpp"
#include "dto/ProgressResponse.hpp"

namespace als {

nlohmann::json AnalyticsService::performance(int64_t userId) {
    auto summary = PerformanceAnalyzer::analyze(answers_.recentAttempts(userId, 500));
    nlohmann::json j = toJson(summary);
    j["current_level"] = progress_.ensureProfile(userId, cfg::kStartLevel).currentLevel;
    return j;
}

nlohmann::json AnalyticsService::weakTopics(int64_t userId) {
    auto all = progress_.all(userId);
    std::sort(all.begin(), all.end(), [](const LearningProgress& a, const LearningProgress& b) {
        return a.masteryScore < b.masteryScore;
    });
    nlohmann::json topics = nlohmann::json::array();
    for (const auto& p : all) {
        nlohmann::json t = toJson(p);
        t["is_weak"] = p.masteryScore < cfg::kWeakTopicThreshold;
        topics.push_back(t);
    }
    return {{"threshold", cfg::kWeakTopicThreshold}, {"topics", topics}};
}

nlohmann::json AnalyticsService::studentsOverview() {
    nlohmann::json rows = nlohmann::json::array();
    for (const auto& u : users_.listByRole("student")) {
        auto prog = progress_.all(u.id);
        int attempts = 0, correct = 0;
        double masterySum = 0;
        std::string weakest;
        double weakestScore = 2.0;
        for (const auto& p : prog) {
            attempts += p.totalAttempts;
            correct += p.correctAttempts;
            masterySum += p.masteryScore;
            if (p.masteryScore < weakestScore) { weakestScore = p.masteryScore; weakest = p.topic; }
        }
        rows.push_back({{"id", u.id}, {"name", u.username}, {"attempts", attempts},
                        {"accuracy", attempts ? static_cast<double>(correct) / attempts : 0.0},
                        {"average_mastery", prog.empty() ? 0.0 : masterySum / prog.size()},
                        {"weakest_topic", weakest},
                        {"level", progress_.ensureProfile(u.id, cfg::kStartLevel).currentLevel}});
    }
    return {{"students", rows}};
}

}  // namespace als
