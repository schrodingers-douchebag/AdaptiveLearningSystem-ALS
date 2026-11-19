#include "CoinChange.h"
#include <algorithm>
#include <limits>

CoinChangeResult minimumCoinChange(const std::vector<int>& denominations, int amount) {
    if (amount < 0) return {0, {}, false};
    const int INF = std::numeric_limits<int>::max() / 4;
    std::vector<int> dp(amount + 1, INF), last(amount + 1, -1);
    dp[0] = 0;
    for (int x = 1; x <= amount; ++x) {
        for (int coin : denominations) {
            if (coin > 0 && coin <= x && dp[x - coin] + 1 < dp[x]) {
                dp[x] = dp[x - coin] + 1;
                last[x] = coin;
            }
        }
    }
    if (dp[amount] == INF) return {0, {}, false};
    std::vector<int> used;
    for (int x = amount; x > 0; x -= last[x]) used.push_back(last[x]);
    return {dp[amount], used, true};
}
