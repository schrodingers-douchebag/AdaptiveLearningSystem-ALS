#!/usr/bin/env bash
# One-time setup for Fedora: installs system packages, builds the backend, installs frontend packages.
# Run from anywhere:  ./scripts/fedora-setup.sh      (asks for your sudo password once)
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

echo "==> 1/3 Installing system packages (sudo dnf)"
sudo dnf install -y gcc-c++ cmake ninja-build pkgconf-pkg-config sqlite-devel libsodium-devel git nodejs npm

echo "==> 2/3 Building the backend (the first build downloads Crow, Asio, spdlog, nlohmann/json: needs internet)"
cd "$ROOT/backend"
cmake -S . -B build -G Ninja -DALS_BUILD_TESTS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build -j"$(nproc)"

echo "==> 3/3 Installing frontend packages"
cd "$ROOT/frontend"
npm install

echo
echo "Done. Next: terminal 1 -> ./scripts/start-backend.sh   terminal 2 -> ./scripts/start-frontend.sh"
