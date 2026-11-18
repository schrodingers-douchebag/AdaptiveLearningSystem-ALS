#include <gtest/gtest.h>
#include "database/Database.hpp"
#include "database/DatabaseInitializer.hpp"
#include "services/AuthService.hpp"
#include "utils/AppError.hpp"
#include "utils/PasswordHasher.hpp"

using namespace als;

struct AuthFixture : ::testing::Test {
    Database db{":memory:"};
    UserRepository users{db};
    ProgressRepository progress{db};
    JwtManager jwt{"test-secret", 5};
    AuthService auth{users, progress, jwt};
    void SetUp() override { DatabaseInitializer(db).initialize(); }
};

TEST(PasswordHasher, HashesAndVerifies) {
    auto h = PasswordHasher::hash("Secret123");
    EXPECT_NE(h, "Secret123");
    EXPECT_TRUE(PasswordHasher::verify(h, "Secret123"));
    EXPECT_FALSE(PasswordHasher::verify(h, "secret123"));
}
TEST_F(AuthFixture, RegisterThenLogin) {
    auto reg = auth.registerStudent("alice", "alice@example.com", "Passw0rdX");
    EXPECT_EQ(reg.user.role, "student");
    auto login = auth.login("alice", "Passw0rdX");
    EXPECT_EQ(login.user.id, reg.user.id);
    auto claims = jwt.verify(login.token);
    ASSERT_TRUE(claims.has_value());
    EXPECT_EQ(claims->userId, reg.user.id);
    EXPECT_EQ(claims->role, "student");
}
TEST_F(AuthFixture, PasswordIsNotStoredInPlainText) {
    auto reg = auth.registerStudent("bob", "bob@example.com", "Passw0rdX");
    EXPECT_NE(reg.user.passwordHash.find("$argon2id$"), std::string::npos);
}
TEST_F(AuthFixture, DuplicateUsernameIsRejected) {
    auth.registerStudent("carol", "carol@example.com", "Passw0rdX");
    try { auth.registerStudent("CAROL", "other@example.com", "Passw0rdX"); FAIL(); }
    catch (const AppError& e) { EXPECT_EQ(e.status(), 409); }
}
TEST_F(AuthFixture, WeakPasswordAndBadEmailAreRejected) {
    EXPECT_THROW(auth.registerStudent("dave", "dave@example.com", "short1"), AppError);
    EXPECT_THROW(auth.registerStudent("dave", "dave@example.com", "onlyletters"), AppError);
    EXPECT_THROW(auth.registerStudent("dave", "not-an-email", "Passw0rdX"), AppError);
}
TEST_F(AuthFixture, WrongPasswordGivesGenericError) {
    auth.registerStudent("erin", "erin@example.com", "Passw0rdX");
    try { auth.login("erin", "wrongpass1"); FAIL(); } catch (const AppError& e) { EXPECT_EQ(e.status(), 401); }
    try { auth.login("nobody", "wrongpass1"); FAIL(); } catch (const AppError& e) { EXPECT_EQ(e.status(), 401); }
}
TEST(Jwt, TamperedAndWrongSecretTokensAreRejected) {
    JwtManager a("secret-a", 5), b("secret-b", 5);
    User u; u.id = 7; u.username = "x"; u.role = "student";
    std::string token = a.issue(u);
    EXPECT_TRUE(a.verify(token).has_value());
    EXPECT_FALSE(b.verify(token).has_value());
    std::string tampered = token;
    tampered[tampered.size() / 2] = tampered[tampered.size() / 2] == 'A' ? 'B' : 'A';
    EXPECT_FALSE(a.verify(tampered).has_value());
    EXPECT_FALSE(a.verify("garbage").has_value());
}
TEST(Jwt, ExpiredTokenIsRejected) {
    JwtManager a("secret-a", -1);  // already expired
    User u; u.id = 7; u.role = "student";
    EXPECT_FALSE(a.verify(a.issue(u)).has_value());
}
