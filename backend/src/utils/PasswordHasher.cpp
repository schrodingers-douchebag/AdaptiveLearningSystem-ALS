#include "utils/PasswordHasher.hpp"

#include <sodium.h>
#include <stdexcept>

namespace als {

static void ensureSodium() {
    static const bool ok = (sodium_init() >= 0);
    if (!ok) throw std::runtime_error("libsodium failed to initialise");
}

std::string PasswordHasher::hash(const std::string& password) {
    ensureSodium();
    char out[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(out, password.c_str(), password.size(), crypto_pwhash_OPSLIMIT_INTERACTIVE,
                          crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
        throw std::runtime_error("password hashing ran out of memory");
    return out;
}

bool PasswordHasher::verify(const std::string& hash, const std::string& password) {
    ensureSodium();
    return crypto_pwhash_str_verify(hash.c_str(), password.c_str(), password.size()) == 0;
}

}  // namespace als
