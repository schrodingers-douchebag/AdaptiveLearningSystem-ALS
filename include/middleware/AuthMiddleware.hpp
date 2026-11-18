#pragma once
#include <crow.h>
#include <string>
#include "utils/AppError.hpp"
#include "utils/JwtManager.hpp"

namespace als {

struct AuthContext {
    int64_t userId = 0;
    std::string username;
    std::string role;
};

// Reads "Authorization: Bearer <token>". Throws 401 (no/bad token) or 403 (wrong role).
inline AuthContext requireAuth(const crow::request& req, const JwtManager& jwt, const std::string& role = "") {
    const std::string header = req.get_header_value("Authorization");
    const std::string prefix = "Bearer ";
    if (header.rfind(prefix, 0) != 0) throw AppError(401, "Missing or malformed Authorization header");

    auto claims = jwt.verify(header.substr(prefix.size()));
    if (!claims) throw AppError(401, "Invalid or expired token");
    if (!role.empty() && claims->role != role) throw AppError(403, "You do not have access to this resource");
    return {claims->userId, claims->username, claims->role};
}

}  // namespace als
