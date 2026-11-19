# AdaptiveLearningSystem (ALS)

An adaptive learning web app: students practise questions that adapt to their level; teachers manage courses, study materials and question banks.

```
AdaptiveLearningSystem-ALS/
├── backend/          C++17 REST API (Crow + SQLite + JWT), port 8080
├── frontend/         React + Vite web app, port 5173
├── algorithms-cli/   Standalone C++ console demo of the course algorithms (not connected to the web app)
├── sample-data/      Example question file and study-material file for uploads
└── scripts/          fedora-setup.sh, start-backend.sh, start-frontend.sh, run-tests.sh
```

## Run it on Fedora (Workstation)

One time:
1. Open a terminal in this folder (`AdaptiveLearningSystem-ALS`).
2. Run `./scripts/fedora-setup.sh` (installs packages with `sudo dnf`, builds the backend, runs `npm install`). The first build needs internet and takes a few minutes.

Every time (two terminals, both started from this folder):
1. Terminal 1: `./scripts/start-backend.sh`. Wait for `API listening on http://localhost:8080`.
2. Terminal 2: `./scripts/start-frontend.sh`. Wait for `Local: http://localhost:5173/`.
3. Open `http://localhost:5173` in a browser.

Check the API alone: `curl http://localhost:8080/api/health` should print `"status":"ok"`.

Stop either one with `Ctrl+C`.

## Accounts
| Role | Username | Password |
|---|---|---|
| Teacher | `teacher` | `Teacher#123` |
| Student | `rafi` | `Student#123` |

Anyone can register as a student on the login page. Teachers only exist as the seeded demo account. Change both demo passwords and set `"seed_demo_users": false` in `backend/config/config.json` before any real use.

## What to try
1. Log in as `teacher`, open **Courses**, create a course named `Operating Systems`.
2. Open it. Under **Add study material** choose `sample-data/material-operating-systems.txt`, then **Save material**.
3. Under **Import questions from a .txt file** choose `sample-data/questions-operating-systems.txt`. The page says `Imported 5 questions.`
4. Log out, log in as `rafi`. **Courses** shows the material. **Practice** serves adaptive questions (including the new ones). **Dashboard** shows topic performance.
5. Log in as `teacher` again. **Students** shows rafi's accuracy, weakest topic and level.

## Question file format
Blocks separated by a blank line. `TOPIC` (default `General`) and `DIFFICULTY` 1-5 (default 2) are optional. Lines starting with `#` are comments. If any block is invalid, nothing is saved and the error names the block.
```
TOPIC: Arrays
DIFFICULTY: 2
Q: What is the index of the first element of a C array?
A) 1
B) 0
C) -1
ANSWER: B
```

## Settings you may need
- **Different frontend address or port:** edit `"cors_origin"` in `backend/config/config.json` to match, then restart the backend.
- **Different backend address or port:** edit `VITE_API_URL` in `frontend/.env`, then restart the frontend. The backend port is `"port"` in `config.json` (or the `PORT` environment variable).
- **Your own JWT secret:** `export JWT_SECRET="..."` before `./scripts/start-backend.sh`. Otherwise one is generated once into `.jwt_secret` in this folder.
- **Reset all data:** stop the backend, delete `backend/data/adaptive_learning.db*`, start it again.

## Tests
- Backend: `./scripts/run-tests.sh` (38 tests).
- Against a running backend: `bash backend/scripts/smoke_test.sh`.
- Console algorithms demo: `cd algorithms-cli && g++ -std=c++17 -Iinclude src/main.cpp src/algorithms/*.cpp src/core/*.cpp src/models/*.cpp -o adaptive_learning && ./adaptive_learning`.

## What is real and what is still mock
Connected to the backend: login/register, practice quiz, topic performance, teacher student table, courses, materials, question import.
Still sample data in the frontend (the backend has no endpoints for them yet): the Activity chart, Progress ring, Subjects card, to-do list, calendar and the Study plan knapsack (`frontend/src/utils/knapsack.js`, the JavaScript version of `algorithms-cli`'s knapsack).

## Known limits
Single-worker server with one SQLite connection (fine for a class demo, not for heavy load). No rate limiting, no token refresh. Uploads are plain `.txt` only (materials up to 200 KB, question files up to 300 KB).
