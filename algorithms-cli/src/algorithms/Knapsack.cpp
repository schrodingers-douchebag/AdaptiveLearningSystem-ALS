#include "Knapsack.h"
#include <algorithm>

std::vector<Question> selectQuestions01(const std::vector<Question>& q, int capacity) {
    if (capacity <= 0 || q.empty()) return {};
    const int n = static_cast<int>(q.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        const int wt = std::max(0, q[i - 1].estimatedMinutes);
        const int val = q[i - 1].score;
        for (int w = 0; w <= capacity; ++w) {
            dp[i][w] = dp[i - 1][w];
            if (wt <= w) dp[i][w] = std::max(dp[i][w], val + dp[i - 1][w - wt]);
        }
    }

    std::vector<Question> chosen;
    int w = capacity;
    for (int i = n; i >= 1; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            chosen.push_back(q[i - 1]);
            w -= std::max(0, q[i - 1].estimatedMinutes);
        }
    }
    std::reverse(chosen.begin(), chosen.end());
    return chosen;
}
