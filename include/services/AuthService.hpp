#pragma once
#include <string>
#include "repositories/ProgressRepository.hpp"
#include "repositories/UserRepository.hpp"
#include "utils/JwtManager.hpp"

namespace als {
struct AuthResult {
    User user;
    std::string token;
};

class AuthService {
public:
    AuthService(UserRepository& users, ProgressRepository& progress, const JwtManager& jwt)
        : users_(users), progress_(progress), jwt_(jwt) {}

    // Self-registration always creates a student. Teachers are created by the seeder only.
    AuthResult registerStudent(const std::string& username, const std::string& email, const std::string& password);
    AuthResult login(const std::string& usernameOrEmail, const std::string& password);
    User createAccount(const std::string& username, const std::string& email, const std::string& password,
                       const std::string& role);
    void ensureDemoAccounts();

private:
    UserRepository& users_;
    ProgressRepository& progress_;
    const JwtManager& jwt_;
};
}  // namespace als
