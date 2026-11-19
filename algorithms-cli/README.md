# Adaptive Learning & Question Recommendation System

A modular C++17 console application demonstrating algorithms in a practical study-planning system.

## Requirements
- C++17 compiler (g++, clang++, or MSVC)
- CMake 3.16+ (optional but recommended)

## Build with g++
From this folder:

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/algorithms/*.cpp src/core/*.cpp src/models/*.cpp -o adaptive_learning
./adaptive_learning
```

On Windows:
```powershell
g++ -std=c++17 -Iinclude src/main.cpp src/algorithms/*.cpp src/core/*.cpp src/models/*.cpp -o adaptive_learning.exe
.\adaptive_learning.exe
```

## Build with CMake
```bash
cmake -S . -B build
cmake --build build
./build/AdaptiveLearningSystem
```

## Data format
- `data/questions.txt`: `id|topic|difficulty|estimated_minutes|score`
- `data/students.txt`: `id|name`
- `data/progress.txt`: `student_id|topic|completed_question_ids_csv`

The application creates missing data files and seeds sample questions on first run.

## Team workflow
1. Each member works on their assigned `.cpp` and matching `.h` file.
2. Do not rename public functions without telling the integration owner.
3. Compile the whole project after every merge.
4. Add test cases to `tests/algorithm_tests.cpp`.
5. Commit small changes with clear messages.

See `docs/` for team allocation, algorithm explanations, and integration instructions.
