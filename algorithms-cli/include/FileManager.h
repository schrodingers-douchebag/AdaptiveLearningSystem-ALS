#pragma once
#include "Question.h"
#include <string>
#include <vector>
void ensureDataFiles(const std::string& dataDirectory);
std::vector<Question> loadQuestions(const std::string& path);
void saveQuestions(const std::string& path, const std::vector<Question>& questions);
