#pragma once
#include <string>

namespace als {

struct LevelChange {
    int newLevel = 2;
    std::string reason;
};

// Rule table (uses a rolling window, never a single answer):
//   >= 90%   -> level up
//   70-89%   -> level up only with a 2+ correct streak, otherwise keep
//   40-69%   -> keep
//   <  40%   -> level down
class DifficultyAdjuster {
public:
    static LevelChange adjust(int currentLevel, double recentAccuracy, int attemptsInWindow, int correctStreak);
};

}  // namespace als
