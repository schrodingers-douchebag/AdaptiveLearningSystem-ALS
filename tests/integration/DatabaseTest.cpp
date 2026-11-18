#include <gtest/gtest.h>
#include "database/Database.hpp"
#include "database/DatabaseInitializer.hpp"
#include "repositories/AnswerRepository.hpp"
#include "repositories/QuestionRepository.hpp"
#include "repositories/UserRepository.hpp"

using namespace als;

static Question sample() {
    Question q;
    q.subject = "Data Structures"; q.topic = "Stacks"; q.text = "A stack is?"; q.difficulty = 2;
    q.options = {{0, "LIFO", true}, {0, "FIFO", false}, {0, "Sorted", false}};
    return q;
}

TEST(Database, MigrationsAreIdempotent) {
    Database db(":memory:");
    DatabaseInitializer init(db);
    init.initialize();
    EXPECT_NO_THROW(init.initialize());
}
TEST(Database, QuestionRoundTripKeepsOptions) {
    Database db(":memory:");
    DatabaseInitializer(db).initialize();
    QuestionRepository repo(db);
    auto id = repo.insert(sample());
    auto q = repo.findById(id);
    ASSERT_TRUE(q.has_value());
    EXPECT_EQ(q->options.size(), 3u);
    EXPECT_EQ(q->correctAnswer, "LIFO");
    EXPECT_EQ(repo.count(), 1);
}
TEST(Database, RejectsQuestionWithoutExactlyOneCorrectOption) {
    Database db(":memory:");
    DatabaseInitializer(db).initialize();
    QuestionRepository repo(db);
    Question q = sample();
    q.options[1].isCorrect = true;
    EXPECT_THROW(repo.insert(q), std::invalid_argument);
}
TEST(Database, ForeignKeysAreEnforced) {
    Database db(":memory:");
    DatabaseInitializer(db).initialize();
    AnswerRepository answers(db);
    Answer a;
    a.userId = 99; a.questionId = 99; a.selectedOptionId = 99;
    EXPECT_THROW(answers.insert(a), std::runtime_error);
}
TEST(Database, TransactionRollsBackWhenNotCommitted) {
    Database db(":memory:");
    DatabaseInitializer(db).initialize();
    UserRepository users(db);
    {
        Transaction tx(db);
        users.create("temp", "temp@example.com", "hash", "student");
    }  // no commit
    EXPECT_FALSE(users.usernameExists("temp"));
}
