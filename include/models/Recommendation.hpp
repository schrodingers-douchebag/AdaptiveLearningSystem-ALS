#pragma once
#include <cstdint>
#include <string>

namespace als {
struct Recommendation {
    int64_t id = 0;
    int64_t userId = 0;
    int64_t questionId = 0;
    double score = 0.0;
    std::string createdAt;
};
}  // namespace als
