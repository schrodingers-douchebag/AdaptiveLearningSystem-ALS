#pragma once
#include <vector>
struct StudyActivity {
    int id{};
    int start{};
    int finish{};
};
std::vector<StudyActivity> selectNonOverlappingActivities(std::vector<StudyActivity> activities);
