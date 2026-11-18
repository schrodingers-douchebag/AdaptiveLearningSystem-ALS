#pragma once

#include <string>
#include <vector>

struct Student {
    int id{};
    std::string name;

    // IDs of questions already completed
    std::vector<int> completedQuestionIds;

    // Topics the student needs more practice in
    std::vector<std::string> weakTopics;
};