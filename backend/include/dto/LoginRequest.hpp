#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace als {
struct LoginRequest {
    std::string identifier;  // username or email
    std::string password;
    static LoginRequest fromJson(const nlohmann::json& j) {
        std::string id = j.value("username", "");
        if (id.empty()) id = j.value("email", "");
        return {id, j.value("password", "")};
    }
};
}  // namespace als
