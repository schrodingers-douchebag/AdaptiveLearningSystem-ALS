#include "FractionalKnapsack.h"
#include <algorithm>

std::vector<TopicAllocation> allocateStudyTimeFractional(
    const std::vector<TopicStudy>& topics, double capacity) {
    std::vector<TopicStudy> sorted = topics;
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        const double ra = a.minutes > 0 ? a.value / a.minutes : 0;
        const double rb = b.minutes > 0 ? b.value / b.minutes : 0;
        return ra > rb;
    });

    std::vector<TopicAllocation> result;
    for (const auto& t : sorted) {
        if (capacity <= 0) break;
        if (t.minutes <= 0 || t.value <= 0) continue;
        const double take = std::min(capacity, t.minutes);
        result.push_back({t.id, take, take / t.minutes});
        capacity -= take;
    }
    return result;
}
