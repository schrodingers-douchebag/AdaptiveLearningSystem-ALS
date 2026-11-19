#include "algorithms/DifficultyAdjuster.hpp"

#include <algorithm>
#include "algorithms/AlgorithmConfig.hpp"

namespace als {

LevelChange DifficultyAdjuster::adjust(int level, double acc, int attempts, int streak) {
    level = std::clamp(level, cfg::kMinLevel, cfg::kMaxLevel);
    LevelChange out{level, ""};

    if (attempts < cfg::kMinAttemptsToAdjust) {
        out.reason = "Not enough recent answers yet, keeping the level";
        return out;
    }

    int delta = 0;
    if (acc >= 0.90) {
        delta = +1;
        out.reason = "Excellent recent accuracy, moving up";
    } else if (acc >= 0.70) {
        if (streak >= 2) {
            delta = +1;
            out.reason = "Good accuracy and a streak, moving up";
        } else {
            out.reason = "Good accuracy, holding the level";
        }
    } else if (acc >= 0.40) {
        out.reason = "Mixed results, holding the level";
    } else {
        delta = -1;
        out.reason = "Struggling recently, easing off";
    }

    out.newLevel = std::clamp(level + delta, cfg::kMinLevel, cfg::kMaxLevel);
    if (out.newLevel == level && delta != 0) out.reason += " (already at the limit)";
    return out;
}

}  // namespace als
