# Integration guide

## Shared interface contract
Headers in `include/` are the contract between team members. Implement the function declared in the matching header; do not change parameter or return types without team agreement.

## Merge order
1. Confirm all headers compile.
2. Merge algorithm implementations.
3. Merge FileManager and sample data.
4. Merge main menu.
5. Build using both the g++ command and CMake.
6. Run each menu option and capture screenshots for the report.

## Git workflow (recommended)
```bash
git checkout -b feature/knapsack
# edit only your assigned files
git add include/Knapsack.h src/algorithms/Knapsack.cpp
git commit -m "Implement 0/1 knapsack question selection"
git push -u origin feature/knapsack
```
Open a pull request and ask one teammate to review it.

## Before submitting
- No compiler warnings that you cannot explain.
- Test normal, boundary, and invalid input.
- Explain why the selected algorithm fits the feature.
- Include pseudocode, complexity, sample input, and sample output in the report.
