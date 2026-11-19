#include "services/AuthService.hpp"

#include "algorithms/AlgorithmConfig.hpp"
#include "utils/AppError.hpp"
#include "utils/Logger.hpp"
#include "utils/PasswordHasher.hpp"
#include "utils/Validator.hpp"

namespace als {

User AuthService::createAccount(const std::string& username, const std::string& email,
                                const std::string& password, const std::string& role) {
    if (!Validator::isValidUsername(username))
        throw AppError(400, "Username must be 3-32 characters: letters, numbers, _ . -");
    if (!Validator::isValidEmail(email)) throw AppError(400, "Email address is not valid");
    if (auto problem = Validator::passwordProblem(password); !problem.empty()) throw AppError(400, problem);
    if (users_.usernameExists(username)) throw AppError(409, "Username is already taken");
    if (users_.emailExists(email)) throw AppError(409, "Email is already registered");

    User u = users_.create(username, email, PasswordHasher::hash(password), role);
    if (role == "student") progress_.ensureProfile(u.id, cfg::kStartLevel);
    return u;
}

AuthResult AuthService::registerStudent(const std::string& username, const std::string& email,
                                        const std::string& password) {
    User u = createAccount(username, email, password, "student");
    Logger::get()->info("New student registered: {}", u.username);
    return {u, jwt_.issue(u)};
}

AuthResult AuthService::login(const std::string& identifier, const std::string& password) {
    if (identifier.empty() || password.empty()) throw AppError(400, "Username and password are required");
    auto user = users_.findByUsernameOrEmail(identifier);
    // Same message for "no such user" and "wrong password" so attackers cannot probe usernames.
    if (!user || !PasswordHasher::verify(user->passwordHash, password))
        throw AppError(401, "Invalid username or password");
    return {*user, jwt_.issue(*user)};
}

void AuthService::ensureDemoAccounts() {
    if (!users_.findByUsernameOrEmail("teacher")) {
        createAccount("teacher", "teacher@example.com", "Teacher#123", "teacher");
        Logger::get()->warn("Demo account created: teacher / Teacher#123 (turn off seed_demo_users in production)");
    }
    if (!users_.findByUsernameOrEmail("rafi")) {
        createAccount("rafi", "rafi@example.com", "Student#123", "student");
        Logger::get()->warn("Demo account created: rafi / Student#123");
    }
}

}  // namespace als
