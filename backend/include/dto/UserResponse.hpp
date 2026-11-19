#pragma once
#include <nlohmann/json.hpp>
#include "models/User.hpp"

namespace als {
// Never includes the password hash.
inline nlohmann::json toJson(const User& u) {
    return {{"id", u.id}, {"username", u.username}, {"email", u.email}, {"role", u.role}, {"created_at", u.createdAt}};
}
}  // namespace als
