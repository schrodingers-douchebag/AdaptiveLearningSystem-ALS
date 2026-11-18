#pragma once
#include <vector>
#include "database/Database.hpp"
#include "models/Recommendation.hpp"

namespace als {
class RecommendationRepository {
public:
    explicit RecommendationRepository(Database& db) : db_(db) {}
    void save(int64_t userId, int64_t questionId, double score);
    std::vector<Recommendation> recent(int64_t userId, int limit);

private:
    Database& db_;
};
}  // namespace als
