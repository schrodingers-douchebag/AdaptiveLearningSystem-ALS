#pragma once
#include <cstdint>
#include <string>

namespace als {
// One row of answer_history.
struct Answer {
    int64_t id = 0;
    int64_t userId = 0;
    int64_t questionId = 0;
    int64_t selectedOptionId = 0;
    bool isCorrect = false;
    int timeTaken = 0;  // seconds
    std::string createdAt;
};

// Light-weight view of a past attempt (answer + the question's topic/difficulty).
// Used by the algorithm layer so it never has to touch the database.
struct AttemptRecord {
    int64_t questionId = 0;
    std::string topic;
    int difficulty = 1;
    bool correct = false;
    int timeTaken = 0;
};
}  // namespace als
