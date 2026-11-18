#pragma once
#include <optional>
#include <string>
#include <vector>
#include "database/Database.hpp"
#include "models/LearningProgress.hpp"
#include "models/StudentProfile.hpp"

namespace als {
class ProgressRepository {
public:
    explicit ProgressRepository(Database& db) : db_(db) {}

    // learning_progress (one row per user + topic)
    std::optional<LearningProgress> get(int64_t userId, const std::string& topic);
    std::vector<LearningProgress> all(int64_t userId);
    void upsert(const LearningProgress& p);

    // student_profiles (difficulty level)
    StudentProfile ensureProfile(int64_t userId, int startLevel);
    void setLevel(int64_t userId, int level);

private:
    Database& db_;
};
}  // namespace als
