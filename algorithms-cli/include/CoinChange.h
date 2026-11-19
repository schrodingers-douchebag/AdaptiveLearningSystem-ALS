#pragma once
#include <vector>
struct CoinChangeResult {
    int minimumCoins{};
    std::vector<int> coinsUsed;
    bool possible{};
};
// Minimum number of coins; coins must be positive. Uses dynamic programming.
CoinChangeResult minimumCoinChange(const std::vector<int>& denominations, int amount);
