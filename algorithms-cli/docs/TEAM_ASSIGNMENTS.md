# Team assignments

## Member 1 — Recommendation / 0-1 Knapsack
Owns:
- `include/Knapsack.h`
- `src/algorithms/Knapsack.cpp`
- Recommendation behavior in `src/core/RecommendationEngine.cpp`

Deliverables:
- Correct DP implementation
- Reconstruction of selected questions
- Tests for zero capacity, exact fit, and impossible/oversized items
- Explain O(nW) time and O(nW) space

## Member 2 — Scheduling / Greedy algorithms
Owns:
- `include/ActivitySelection.h`
- `src/algorithms/ActivitySelection.cpp`
- `include/FractionalKnapsack.h`
- `src/algorithms/FractionalKnapsack.cpp`

Deliverables:
- Activity selection sorted by finish time
- Fractional allocation sorted by value-per-minute
- Tests for overlapping sessions, equal finish times, zero capacity, and invalid durations

## Member 3 — Learning analysis / DP algorithms
Owns:
- `include/LCS.h`
- `src/algorithms/LCS.cpp`
- `include/CoinChange.h`
- `src/algorithms/CoinChange.cpp`

Deliverables:
- LCS length and reconstructed sequence
- Minimum coin count and reconstructed denominations
- Tests for empty strings, no possible change, amount zero, and repeated characters

## Member 4 — Integration / persistence / graph extension
Owns:
- `src/main.cpp`
- `include/FileManager.h`
- `src/core/FileManager.cpp`
- `include/BellmanFord.h`
- `src/algorithms/BellmanFord.cpp`
- CMake and integration tests

Deliverables:
- Build the full project
- Load/save sample files
- Validate input
- Test unreachable vertices and negative cycles
- Coordinate merges and final demonstration

## Team rule
Each member must make a branch or separate copy, preserve the public function signatures, and send the integration owner a tested change. Merge one module at a time.
