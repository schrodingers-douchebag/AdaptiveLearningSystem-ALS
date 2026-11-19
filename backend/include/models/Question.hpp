#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace als {
struct AnswerOption {
    int64_t id = 0;
    std::string text;
    bool isCorrect = false;
};

struct Question {
    int64_t id = 0;
    std::string subject;
    std::string topic;
    std::string text;
    int difficulty = 1;  // 1 (easy) .. 5 (hard)
    std::string correctAnswer;
    std::vector<AnswerOption> options;  // empty when loaded in "light" mode
};
}  // namespace als
