# Algorithm explanations and project mapping

| Algorithm | Project use | Core idea | Typical complexity |
|---|---|---|---|
| 0/1 Knapsack | Pick questions within a time budget | DP over items and capacity | O(nW) |
| Activity Selection | Pick maximum non-overlapping sessions | Sort by finish time, then greedily select | O(n log n) |
| Fractional Knapsack | Allocate partial study time by value/minute | Sort by ratio and take fractions | O(n log n) |
| LCS | Compare ordered topic/practice sequences | 2D dynamic programming | O(nm) |
| Coin Change | Represent a duration with minimum time units | DP over amounts | O(Ak) |
| Bellman-Ford | Shortest path through topic prerequisites | Relax every edge V-1 times | O(VE) |

## Important academic caveats
- 0/1 Knapsack is appropriate when each question is either assigned or not assigned.
- Fractional Knapsack is a time-allocation model, not a claim that a question itself can be partially solved.
- Activity Selection assumes sessions have fixed start and finish times.
- LCS compares sequences; it does not independently diagnose mastery.
- Coin Change is meaningful only when the chosen denominations represent allowed study blocks.
- Bellman-Ford is an optional extension. Negative edge weights should represent a meaningful cost model; ordinary prerequisite graphs usually use nonnegative weights.

## Suggested demo inputs
- Knapsack: 30 minutes; show selected questions and total value.
- Activity Selection: overlapping sessions; show compatible sessions.
- Fractional Knapsack: 40 minutes across topics with different value/minute ratios.
- LCS: compare two ordered topic sequences.
- Coin Change: represent 28 minutes using {1, 5, 10, 15}.
- Bellman-Ford: demonstrate shortest paths and explain negative-cycle detection.
