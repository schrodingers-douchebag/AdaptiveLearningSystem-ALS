#include "ActivitySelection.h"
#include <algorithm>

std::vector<StudyActivity> selectNonOverlappingActivities(std::vector<StudyActivity> a) {
    std::sort(a.begin(), a.end(), [](const auto& x, const auto& y) {
        if (x.finish != y.finish) return x.finish < y.finish;
        return x.start < y.start;
    });
    std::vector<StudyActivity> chosen;
    int lastFinish = -1;
    for (const auto& activity : a) {
        if (activity.start >= lastFinish && activity.finish >= activity.start) {
            chosen.push_back(activity);
            lastFinish = activity.finish;
        }
    }
    return chosen;
}
