#include <gtest/gtest.h>
#include "utils/QuestionImporter.hpp"

using namespace als;

namespace {
const char* kValid =
    "# comment\n"
    "TOPIC: Arrays\n"
    "DIFFICULTY: 3\n"
    "Q: What is the index of the first element?\n"
    "A) 1\n"
    "B) 0\n"
    "C) -1\n"
    "ANSWER: B\n"
    "\n"
    "Q: Stack order?\n"
    "A. FIFO\n"
    "B. LIFO\n"
    "ANSWER: b\n";
}

TEST(QuestionImporter, ParsesValidBlocksAndDefaults) {
    auto r = parseQuestionText(kValid, "Data Structures");
    ASSERT_TRUE(r.errors.empty());
    ASSERT_EQ(r.questions.size(), 2u);
    EXPECT_EQ(r.questions[0].subject, "Data Structures");
    EXPECT_EQ(r.questions[0].topic, "Arrays");
    EXPECT_EQ(r.questions[0].difficulty, 3);
    ASSERT_EQ(r.questions[0].options.size(), 3u);
    EXPECT_TRUE(r.questions[0].options[1].isCorrect);
    EXPECT_EQ(r.questions[1].topic, "General");
    EXPECT_EQ(r.questions[1].difficulty, 2);
    EXPECT_TRUE(r.questions[1].options[1].isCorrect);
}

TEST(QuestionImporter, AcceptsWindowsLineEndings) {
    std::string text = "Q: One?\r\nA) x\r\nB) y\r\nANSWER: A\r\n";
    auto r = parseQuestionText(text, "S");
    EXPECT_TRUE(r.errors.empty());
    ASSERT_EQ(r.questions.size(), 1u);
    EXPECT_EQ(r.questions[0].options[1].text, "y");
}

TEST(QuestionImporter, ReportsEveryBadBlockWithItsNumber) {
    auto r = parseQuestionText("Q: ok?\nA) a\nB) b\nANSWER: A\n\nQ: no answer\nA) a\nB) b\n\nQ: bad letter\nA) a\nB) b\nANSWER: D\n", "S");
    ASSERT_EQ(r.errors.size(), 2u);
    EXPECT_NE(r.errors[0].find("Block 2"), std::string::npos);
    EXPECT_NE(r.errors[1].find("Block 3"), std::string::npos);
}

TEST(QuestionImporter, RejectsTooFewOptionsBadDifficultyAndEmptyText) {
    EXPECT_FALSE(parseQuestionText("Q: x\nA) only\nANSWER: A\n", "S").errors.empty());
    EXPECT_FALSE(parseQuestionText("DIFFICULTY: 9\nQ: x\nA) a\nB) b\nANSWER: A\n", "S").errors.empty());
    EXPECT_FALSE(parseQuestionText("   \n\n", "S").errors.empty());
}
