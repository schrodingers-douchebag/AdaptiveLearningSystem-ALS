
#pragma once

#include "Question.h"
#include "Student.h"

#include <vector>

std::vector<Question> recommendQuestions(
    const std::vector<Question>& bank,
    const Student& student,
    int minutes
);
