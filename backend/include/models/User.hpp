#pragma once
#include <cstdint>
#include <string>

namespace als {
struct User {
    int64_t id = 0;
    std::string username;
    std::string email;
    std::string passwordHash;
    std::string role;  // "student" or "teacher"
    std::string createdAt;
};
}  // namespace als
