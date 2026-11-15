#pragma once
#include <vector>
struct TopicStudy {
    int id{};
    double value{};
    double minutes{};
};
struct TopicAllocation {
    int id{};
    double minutesAllocated{};
    double fraction{};
};
std::vector<TopicAllocation> allocateStudyTimeFractional(
    const std::vector<TopicStudy>& topics, double availableMinutes);
