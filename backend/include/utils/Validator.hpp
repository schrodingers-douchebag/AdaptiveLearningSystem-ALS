#pragma once
#include <string>

namespace als {
class Validator {
public:
    static bool isValidUsername(const std::string& s);  // 3-32 chars: letters, digits, _ . -
    static bool isValidEmail(const std::string& s);
    // Returns "" when the password is fine, otherwise a human-readable reason.
    static std::string passwordProblem(const std::string& s);
    static int clamp(int value, int lo, int hi);
};
}  // namespace als
