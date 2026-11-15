#pragma once
#include <string>
#include <vector>

struct Student {
    int id{};
    std::string name;
    std::vector<int> completedQuestionIds;
};
