#!/usr/bin/env bash
# Starts the API on http://localhost:8080 (keep this terminal open).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT/backend"   # the server must start from backend/ so config/ and data/ are found

if [ ! -x build/AdaptiveLearningSystem ]; then
  echo "The backend is not built yet. Run ./scripts/fedora-setup.sh first." >&2
  exit 1
fi

# Use JWT_SECRET from your environment if set; otherwise create one once and reuse it.
if [ -z "${JWT_SECRET:-}" ]; then
  SECRET_FILE="$ROOT/.jwt_secret"
  if [ ! -f "$SECRET_FILE" ]; then
    head -c 48 /dev/urandom | base64 | tr -d '\n' > "$SECRET_FILE"
    chmod 600 "$SECRET_FILE"
  fi
  JWT_SECRET="$(cat "$SECRET_FILE")"
fi
export JWT_SECRET
exec ./build/AdaptiveLearningSystem
