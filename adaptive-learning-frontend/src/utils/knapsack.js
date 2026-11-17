// 0/1 Knapsack (DP). weight = minutes, value = priority, capacity = free minutes.
export function knapsack(items, capacity) {
  const n = items.length;
  const dp = Array.from({ length: n + 1 }, () => Array(capacity + 1).fill(0));

  for (let i = 1; i <= n; i++) {
    const { minutes, priority } = items[i - 1];
    for (let w = 0; w <= capacity; w++) {
      dp[i][w] = dp[i - 1][w];                       // skip item
      if (minutes <= w) {
        dp[i][w] = Math.max(dp[i][w], dp[i - 1][w - minutes] + priority); // take item
      }
    }
  }

  // walk back to find which items were picked
  const picked = [];
  let w = capacity;
  for (let i = n; i > 0; i--) {
    if (dp[i][w] !== dp[i - 1][w]) {
      picked.push(items[i - 1]);
      w -= items[i - 1].minutes;
    }
  }
  return { picked: picked.reverse(), totalValue: dp[n][capacity] };
}
