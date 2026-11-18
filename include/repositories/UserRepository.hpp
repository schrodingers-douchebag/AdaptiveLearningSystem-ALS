#pragma once
#include <optional>
#include <string>
#include <vector>
#include "database/Database.hpp"
#include "models/User.hpp"

namespace als {
class UserRepository {
public:
    explicit UserRepository(Database& db) : db_(db) {}

    User create(const std::string& username, const std::string& email, const std::string& passwordHash,
                const std::string& role);
    std::optional<User> findById(int64_t id);
    std::optional<User> findByUsernameOrEmail(const std::string& value);
    bool usernameExists(const std::string& username);
    bool emailExists(const std::string& email);
    std::vector<User> listByRole(const std::string& role);

private:
    Database& db_;
};
}  // namespace als
