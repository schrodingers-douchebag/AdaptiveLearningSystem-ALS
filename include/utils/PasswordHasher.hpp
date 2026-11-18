#pragma once
#include <string>

namespace als {
// Argon2id through libsodium. The salt and parameters live inside the hash string.
class PasswordHasher {
public:
    static std::string hash(const std::string& password);
    static bool verify(const std::string& hash, const std::string& password);
};
}  // namespace als
