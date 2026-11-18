#include "utils/JwtManager.hpp"

#include <sodium.h>
#include <array>
#include <chrono>
#include <nlohmann/json.hpp>
#include <stdexcept>

namespace als {
namespace {

using json = nlohmann::json;
constexpr int kB64 = sodium_base64_VARIANT_URLSAFE_NO_PADDING;

std::string b64encode(const std::string& in) {
    std::string out(sodium_base64_ENCODED_LEN(in.size(), kB64), '\0');
    sodium_bin2base64(out.data(), out.size(), reinterpret_cast<const unsigned char*>(in.data()), in.size(), kB64);
    out.resize(std::char_traits<char>::length(out.c_str()));  // drop the trailing NUL
    return out;
}

bool b64decode(const std::string& in, std::string& out) {
    out.assign(in.size(), '\0');
    size_t len = 0;
    const char* end = nullptr;
    if (sodium_base642bin(reinterpret_cast<unsigned char*>(out.data()), out.size(), in.data(), in.size(),
                          nullptr, &len, &end, kB64) != 0 || end != in.data() + in.size())
        return false;
    out.resize(len);
    return true;
}

// Any-length secret -> fixed 32-byte HMAC key.
std::array<unsigned char, crypto_auth_hmacsha256_KEYBYTES> deriveKey(const std::string& secret) {
    std::array<unsigned char, crypto_hash_sha256_BYTES> key{};
    crypto_hash_sha256(key.data(), reinterpret_cast<const unsigned char*>(secret.data()), secret.size());
    return key;
}

std::string header() { return b64encode(R"({"alg":"HS256","typ":"JWT"})"); }

int64_t nowSeconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

}  // namespace

JwtManager::JwtManager(std::string secret, int ttlMinutes) : secret_(std::move(secret)), ttlMinutes_(ttlMinutes) {
    if (sodium_init() < 0) throw std::runtime_error("libsodium failed to initialise");
}

std::string JwtManager::issue(const User& user) const {
    const int64_t now = nowSeconds();
    json payload = {{"sub", user.id}, {"username", user.username}, {"role", user.role},
                    {"iat", now}, {"exp", now + static_cast<int64_t>(ttlMinutes_) * 60}};
    std::string signingInput = header() + "." + b64encode(payload.dump());

    auto key = deriveKey(secret_);
    std::array<unsigned char, crypto_auth_hmacsha256_BYTES> mac{};
    crypto_auth_hmacsha256(mac.data(), reinterpret_cast<const unsigned char*>(signingInput.data()),
                           signingInput.size(), key.data());
    return signingInput + "." + b64encode(std::string(mac.begin(), mac.end()));
}

std::optional<JwtClaims> JwtManager::verify(const std::string& token) const {
    auto d1 = token.find('.');
    auto d2 = token.find('.', d1 == std::string::npos ? 0 : d1 + 1);
    if (d1 == std::string::npos || d2 == std::string::npos) return std::nullopt;

    // The header must be exactly ours: rejects "alg":"none" tricks.
    if (token.substr(0, d1) != header()) return std::nullopt;

    std::string signingInput = token.substr(0, d2);
    std::string macBytes;
    if (!b64decode(token.substr(d2 + 1), macBytes) || macBytes.size() != crypto_auth_hmacsha256_BYTES)
        return std::nullopt;

    auto key = deriveKey(secret_);
    if (crypto_auth_hmacsha256_verify(reinterpret_cast<const unsigned char*>(macBytes.data()),
                                      reinterpret_cast<const unsigned char*>(signingInput.data()),
                                      signingInput.size(), key.data()) != 0)
        return std::nullopt;  // constant-time check failed

    std::string payloadText;
    if (!b64decode(token.substr(d1 + 1, d2 - d1 - 1), payloadText)) return std::nullopt;
    json p = json::parse(payloadText, nullptr, false);
    if (!p.is_object() || !p.contains("sub") || !p.contains("exp")) return std::nullopt;

    JwtClaims c;
    c.userId = p.value("sub", int64_t{0});
    c.username = p.value("username", "");
    c.role = p.value("role", "");
    c.expiresAt = p.value("exp", int64_t{0});
    if (c.expiresAt <= nowSeconds() || c.userId <= 0) return std::nullopt;
    return c;
}

}  // namespace als
