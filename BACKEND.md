# AdaptiveLearningSystem — backend

C++17 · CMake · SQLite · Crow (REST) · nlohmann/json · libsodium (Argon2id + HMAC) · spdlog · GoogleTest

## Build and run

**Linux / macOS**
```bash
sudo apt install cmake g++ pkg-config libsqlite3-dev libsodium-dev ninja-build   # Ubuntu/Debian
cmake -S . -B build -G Ninja
cmake --build build -j
./build/tests/als_tests          # 34 tests
export JWT_SECRET="a-long-random-string"
./build/AdaptiveLearningSystem   # run from the project root
```
**Windows (PowerShell)**: `vcpkg install sqlite3 libsodium`, then `.\scripts\build.ps1` and `.\scripts\run.ps1`.
Crow, Asio, nlohmann/json, spdlog and GoogleTest are downloaded automatically by CMake (needs internet the first time).

With the server running: `./scripts/smoke_test.sh` walks through register, login, a learning loop, analytics and CORS.

Demo accounts (created when `seed_demo_users` is true): `teacher / Teacher#123`, `rafi / Student#123`.

## Layers

```
routes (api/) -> services -> algorithms (pure, no DB/HTTP)
                    |
              repositories -> database (SQLite)
```
`algorithms/` only uses `models/`, so it is unit-tested without a database.

## Endpoints  (send `Authorization: Bearer <token>`)

| Method | Path | Who | Purpose |
|---|---|---|---|
| GET | /api/health | anyone | health check |
| POST | /api/auth/register | anyone | create a student |
| POST | /api/auth/login | anyone | get a token |
| GET | /api/users/profile | any user | profile + level |
| GET | /api/questions | any user | list (?topic=&subject=&difficulty=&limit=&offset=) |
| GET | /api/questions/:id | any user | one question |
| POST | /api/questions | teacher | add a question |
| POST | /api/learning/start | student | level + first recommended question |
| POST | /api/learning/submit-answer | student | submit, get feedback + next question |
| GET | /api/learning/progress | student | mastery per topic |
| GET | /api/recommendations/next | student | recommended question + score breakdown |
| GET | /api/analytics/performance | student | accuracy, time, trend, consistency |
| GET | /api/analytics/weak-topics | student | topics sorted weakest first |
| GET | /api/analytics/students | teacher | one row per student |

Answer a question: `POST /api/learning/submit-answer {"question_id":12,"selected_option_id":3,"time_taken":25}`.
Students never receive `is_correct`; the server decides correctness.

## Connect the React frontend
In the frontend `.env`: `VITE_API_URL=http://localhost:8080/api`. CORS already allows `http://localhost:5173` (change `cors_origin` in `config/config.json`).

## Tuning the algorithm
All numbers (weights 0.35/0.30/0.20/0.15, learning rate, windows, thresholds) are in `include/algorithms/AlgorithmConfig.hpp`. They are starting values, not validated ones.

## Known limits
- One worker thread and one SQLite connection (safe and simple, fine for a demo; not for heavy load).
- No rate limiting or token refresh/revocation yet.
- Change `jwt_secret` (env `JWT_SECRET`) and turn off `seed_demo_users` before any real deployment.
