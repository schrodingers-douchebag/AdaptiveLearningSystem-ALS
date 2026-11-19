#pragma once
#include <string>
#include <vector>
#include "models/Answer.hpp"

namespace als {

struct TopicStat {
    std::string topic;
    int attempts = 0;
    int correct = 0;
    double accuracy = 0.0;
};

struct PerformanceSummary {
    int totalAttempts = 0;
    int correct = 0;
    int incorrect = 0;
    double accuracy = 0.0;          // overall, 0..1
    double averageTime = 0.0;       // seconds
    double recentAccuracy = 0.0;    // last kRecentWindow answers
    double previousAccuracy = 0.0;  // the kRecentWindow answers before those
    double trend = 0.0;             // recent - previous
    std::string trendLabel = "not enough data";
    double consistency = 1.0;       // 1 = very steady results, 0 = flip-flopping
    int correctStreak = 0;          // right answers in a row, newest first
    std::vector<TopicStat> topics;  // sorted by accuracy, weakest first
};

class PerformanceAnalyzer {
public:
    // `attempts` must be ordered newest first.
    static PerformanceSummary analyze(const std::vector<AttemptRecord>& attempts);
    static double accuracyOf(const std::vector<AttemptRecord>& attempts, size_t from, size_t count);
};

}  // namespace als
