#include "repositories/UserRepository.hpp"

namespace als {
namespace {
const char* kCols = "id, username, email, password_hash, role, created_at";

User readUser(Statement& st) {
    User u;
    u.id = st.getInt(0);
    u.username = st.getText(1);
    u.email = st.getText(2);
    u.passwordHash = st.getText(3);
    u.role = st.getText(4);
    u.createdAt = st.getText(5);
    return u;
}
}  // namespace

User UserRepository::create(const std::string& username, const std::string& email,
                            const std::string& passwordHash, const std::string& role) {
    db_.prepare("INSERT INTO users(username, email, password_hash, role) VALUES (?, ?, ?, ?);")
        .bind(1, username).bind(2, email).bind(3, passwordHash).bind(4, role).run();
    return *findById(db_.lastInsertId());
}

std::optional<User> UserRepository::findById(int64_t id) {
    auto st = db_.prepare(std::string("SELECT ") + kCols + " FROM users WHERE id = ?;");
    st.bind(1, id);
    if (!st.step()) return std::nullopt;
    return readUser(st);
}

std::optional<User> UserRepository::findByUsernameOrEmail(const std::string& value) {
    auto st = db_.prepare(std::string("SELECT ") + kCols + " FROM users WHERE username = ? OR email = ? LIMIT 1;");
    st.bind(1, value).bind(2, value);
    if (!st.step()) return std::nullopt;
    return readUser(st);
}

bool UserRepository::usernameExists(const std::string& username) {
    auto st = db_.prepare("SELECT 1 FROM users WHERE username = ?;");
    st.bind(1, username);
    return st.step();
}

bool UserRepository::emailExists(const std::string& email) {
    auto st = db_.prepare("SELECT 1 FROM users WHERE email = ?;");
    st.bind(1, email);
    return st.step();
}

std::vector<User> UserRepository::listByRole(const std::string& role) {
    std::vector<User> out;
    auto st = db_.prepare(std::string("SELECT ") + kCols + " FROM users WHERE role = ? ORDER BY id;");
    st.bind(1, role);
    while (st.step()) out.push_back(readUser(st));
    return out;
}

}  // namespace als
