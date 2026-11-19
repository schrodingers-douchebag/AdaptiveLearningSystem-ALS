#!/usr/bin/env bash
# Builds and runs the backend unit/integration tests (downloads GoogleTest the first time).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT/backend"
cmake -S . -B build -G Ninja -DALS_BUILD_TESTS=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build -j"$(nproc)"
./build/tests/als_tests
