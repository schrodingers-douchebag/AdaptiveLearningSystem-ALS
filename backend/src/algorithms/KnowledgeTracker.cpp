#include "algorithms/KnowledgeTracker.hpp"

#include <algorithm>
#include "algorithms/AlgorithmConfig.hpp"

namespace als {

double KnowledgeTracker::learningRate(bool correct, int difficulty) {
    const int d = std::clamp(difficulty, cfg::kMinLevel, cfg::kMaxLevel);
    // correct: 0.8 (easy) .. 1.2 (hard) | wrong: 1.2 (easy) .. 0.8 (hard)
    const double factor = correct ? 0.7 + 0.1 * d : 1.3 - 0.1 * d;
    return std::clamp(cfg::kBaseLearningRate * factor, cfg::kMinLearningRate, cfg::kMaxLearningRate);
}

double KnowledgeTracker::update(double mastery, bool correct, int difficulty) {
    const double target = correct ? 1.0 : 0.0;
    const double next = mastery + learningRate(correct, difficulty) * (target - mastery);
    return std::clamp(next, 0.0, 1.0);
}

}  // namespace als
