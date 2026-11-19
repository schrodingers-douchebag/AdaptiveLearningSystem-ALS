#pragma once
#include <unordered_map>
#include <vector>
#include "database/Database.hpp"
#include "models/Answer.hpp"

namespace als {
class AnswerRepository {
public:
    explicit AnswerRepository(Database& db) : db_(db) {}

    int64_t insert(const Answer& a);
    // Newest attempt first. Joins the question to get topic + difficulty.
    std::vector<AttemptRecord> recentAttempts(int64_t userId, int limit);
    std::unordered_map<int64_t, int> answeredCounts(int64_t userId);  // question id -> times answered
    int64_t totalCount(int64_t userId);

private:
    Database& db_;
};
}  // namespace als
