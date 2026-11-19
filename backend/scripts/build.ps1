# Windows (PowerShell). Needs vcpkg:  vcpkg install sqlite3 libsodium
param([string]$VcpkgRoot = $env:VCPKG_ROOT)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="$VcpkgRoot/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Debug -j 4
