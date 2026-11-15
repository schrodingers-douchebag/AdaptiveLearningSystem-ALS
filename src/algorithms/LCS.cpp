#include "LCS.h"
#include <vector>
#include <algorithm>

std::string longestCommonSubsequence(const std::string& a, const std::string& b) {
    const std::size_t n = a.size(), m = b.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (std::size_t i = 1; i <= n; ++i) {
        for (std::size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    std::string result;
    std::size_t i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result.push_back(a[i - 1]); --i; --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) --i;
        else --j;
    }
    std::reverse(result.begin(), result.end());
    return result;
}
