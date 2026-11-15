// The recommendation engine currently delegates selection to 0/1 Knapsack.
// Extend this file to exclude already-completed questions and weight weak topics.
#include "Knapsack.h"
#include "Question.h"
#include <vector>

std::vector<Question> recommendQuestions(const std::vector<Question>& bank, int minutes) {
    return selectQuestions01(bank, minutes);
}
