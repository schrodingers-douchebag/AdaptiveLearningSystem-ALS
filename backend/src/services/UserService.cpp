#include "services/UserService.hpp"

#include "algorithms/AlgorithmConfig.hpp"
#include "dto/UserResponse.hpp"
#include "utils/AppError.hpp"

namespace als {
nlohmann::json UserService::profile(int64_t userId) {
    auto u = users_.findById(userId);
    if (!u) throw AppError(404, "User not found");
    nlohmann::json j = {{"user", toJson(*u)}};
    if (u->role == "student") {
        j["profile"] = {{"current_level", progress_.ensureProfile(userId, cfg::kStartLevel).currentLevel}};
        j["total_answers"] = answers_.totalCount(userId);
    }
    return j;
}
}  // namespace als
