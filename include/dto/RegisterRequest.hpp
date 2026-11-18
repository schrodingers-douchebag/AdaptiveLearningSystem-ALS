#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace als {
struct RegisterRequest {
    std::string username, email, password;
    static RegisterRequest fromJson(const nlohmann::json& j) {
        return {j.value("username", ""), j.value("email", ""), j.value("password", "")};
    }
};
}  // namespace als
