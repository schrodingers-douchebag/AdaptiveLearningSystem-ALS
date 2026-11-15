#pragma once
#include "Question.h"
#include <vector>

// 0/1 Knapsack: each question is either selected once or not selected.
std::vector<Question> selectQuestions01(const std::vector<Question>& questions, int availableMinutes);
