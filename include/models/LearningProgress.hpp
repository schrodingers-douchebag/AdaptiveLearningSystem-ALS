#pragma once
#include <cstdint>
#include <string>

namespace als {
struct LearningProgress {
    int64_t id = 0;
    int64_t userId = 0;
    std::string topic;
    double masteryScore = 0.5;  // 0..1
    int totalAttempts = 0;
    int correctAttempts = 0;
    std::string updatedAt;
};
}  // namespace als
