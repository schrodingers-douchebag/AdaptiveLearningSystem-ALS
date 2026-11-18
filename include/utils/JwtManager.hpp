#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include "models/User.hpp"

namespace als {

struct JwtClaims {
    int64_t userId = 0;
    std::string username;
    std::string role;
    int64_t expiresAt = 0;  // unix seconds
};

// Minimal HS256 JSON Web Token (HMAC-SHA256) built on libsodium.
class JwtManager {
public:
    JwtManager(std::string secret, int ttlMinutes);
    std::string issue(const User& user) const;
    std::optional<JwtClaims> verify(const std::string& token) const;  // nullopt = bad/expired

private:
    std::string secret_;
    int ttlMinutes_;
};

}  // namespace als
