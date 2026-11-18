#include "algorithms/PerformanceAnalyzer.hpp"

#include <algorithm>
#include <map>
#include "algorithms/AlgorithmConfig.hpp"

namespace als {

double PerformanceAnalyzer::accuracyOf(const std::vector<AttemptRecord>& a, size_t from, size_t count) {
    if (from >= a.size()) return 0.0;
    const size_t end = std::min(a.size(), from + count);
    int right = 0;
    for (size_t i = from; i < end; ++i) right += a[i].correct ? 1 : 0;
    return static_cast<double>(right) / static_cast<double>(end - from);
}

PerformanceSummary PerformanceAnalyzer::analyze(const std::vector<AttemptRecord>& a) {
    PerformanceSummary s;
    s.totalAttempts = static_cast<int>(a.size());
    if (a.empty()) return s;

    std::map<std::string, TopicStat> byTopic;
    double timeSum = 0;
    for (const auto& r : a) {
        s.correct += r.correct ? 1 : 0;
        timeSum += r.timeTaken;
        auto& t = byTopic[r.topic];
        t.topic = r.topic;
        t.attempts++;
        t.correct += r.correct ? 1 : 0;
    }
    s.incorrect = s.totalAttempts - s.correct;
    s.accuracy = static_cast<double>(s.correct) / s.totalAttempts;
    s.averageTime = timeSum / s.totalAttempts;

    const size_t w = cfg::kRecentWindow;
    s.recentAccuracy = accuracyOf(a, 0, w);
    if (a.size() >= w + cfg::kMinAttemptsToAdjust) {
        s.previousAccuracy = accuracyOf(a, w, w);
        s.trend = s.recentAccuracy - s.previousAccuracy;
        s.trendLabel = s.trend > 0.1 ? "improving" : (s.trend < -0.1 ? "declining" : "steady");
    }

    for (const auto& r : a) {
        if (!r.correct) break;
        s.correctStreak++;
    }

    // consistency: how rarely the result flips between right and wrong (last 10 answers)
    const size_t n = std::min<size_t>(a.size(), 10);
    if (n >= 2) {
        int flips = 0;
        for (size_t i = 1; i < n; ++i) flips += (a[i].correct != a[i - 1].correct) ? 1 : 0;
        s.consistency = 1.0 - static_cast<double>(flips) / static_cast<double>(n - 1);
    }

    for (auto& [name, t] : byTopic) {
        t.accuracy = static_cast<double>(t.correct) / t.attempts;
        s.topics.push_back(t);
    }
    std::sort(s.topics.begin(), s.topics.end(), [](const TopicStat& x, const TopicStat& y) {
        return x.accuracy != y.accuracy ? x.accuracy < y.accuracy : x.topic < y.topic;
    });
    return s;
}

}  // namespace als
