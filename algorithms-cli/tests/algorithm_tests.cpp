// Lightweight manual test reference.
// Compile separately with the algorithm source files and add a test framework if desired.
#include "Knapsack.h"
#include "ActivitySelection.h"
#include "FractionalKnapsack.h"
#include "LCS.h"
#include "CoinChange.h"
#include "BellmanFord.h"
#include <cassert>

int main() {
    std::vector<Question> q{{1,"A",1,10,5},{2,"B",2,20,10},{3,"C",1,15,7}};
    auto chosen = selectQuestions01(q, 25);
    int total = 0; for (const auto& x : chosen) total += x.estimatedMinutes;
    assert(total <= 25);

    auto sessions = selectNonOverlappingActivities({{1,1,3},{2,2,4},{3,3,5}});
    assert(!sessions.empty());

    auto allocation = allocateStudyTimeFractional({{1,10,20},{2,9,10}}, 15);
    double allocated = 0; for (const auto& x : allocation) allocated += x.minutesAllocated;
    assert(allocated <= 15.0001);

    assert(longestCommonSubsequence("ABCBDAB", "BDCABA").size() == 4);
    assert(minimumCoinChange({1,5,10}, 18).minimumCoins == 5);

    auto path = bellmanFord(3, {{0,1,4},{1,2,-1}}, 0);
    assert(!path.hasNegativeCycle && path.distances[2] == 3);
}
