#include "utils/Validator.hpp"

#include <algorithm>
#include <cctype>

namespace als {

bool Validator::isValidUsername(const std::string& s) {
    if (s.size() < 3 || s.size() > 32) return false;
    return std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isalnum(c) || c == '_' || c == '.' || c == '-';
    });
}

bool Validator::isValidEmail(const std::string& s) {
    if (s.size() < 5 || s.size() > 254) return false;
    if (std::any_of(s.begin(), s.end(), [](unsigned char c) { return std::isspace(c); })) return false;
    auto at = s.find('@');
    if (at == std::string::npos || at == 0 || s.find('@', at + 1) != std::string::npos) return false;
    auto dot = s.find('.', at);
    return dot != std::string::npos && dot > at + 1 && dot + 1 < s.size();
}

std::string Validator::passwordProblem(const std::string& s) {
    if (s.size() < 8) return "Password must be at least 8 characters";
    if (s.size() > 128) return "Password is too long";
    bool letter = std::any_of(s.begin(), s.end(), [](unsigned char c) { return std::isalpha(c); });
    bool digit = std::any_of(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c); });
    if (!letter || !digit) return "Password must contain at least one letter and one number";
    return "";
}

int Validator::clamp(int v, int lo, int hi) { return std::max(lo, std::min(hi, v)); }

}  // namespace als
