#pragma once
#include <string>

struct Question {
    int id{};
    std::string topic;
    int difficulty{1};       // 1 = easy, 2 = medium, 3 = hard
    int estimatedMinutes{};
    int score{};             // learning value / priority
};
