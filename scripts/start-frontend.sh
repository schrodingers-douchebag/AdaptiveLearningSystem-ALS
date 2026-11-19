#!/usr/bin/env bash
# Starts the web app on http://localhost:5173 (keep this terminal open).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT/frontend"
[ -d node_modules ] || npm install
exec npm run dev
