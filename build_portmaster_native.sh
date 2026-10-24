#!/bin/bash
# Cross-compiles for aarch64 using a locally installed aarch64-linux-gnu toolchain
# (no Docker). The binary's glibc requirement follows the host's arm64 libraries,
# so prefer build_portmaster.sh for release builds.
# Output: ports/eldritch/eldritch/eldritch.aarch64
set -e

REPO_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$REPO_DIR/build-portmaster"
OUTPUT="$REPO_DIR/ports/eldritch/eldritch/eldritch.aarch64"

for tool in aarch64-linux-gnu-gcc aarch64-linux-gnu-strip cmake; do
    if ! command -v "$tool" &>/dev/null; then
        echo "ERROR: $tool not found."
        exit 1
    fi
done

echo "Cross-compiler: $(aarch64-linux-gnu-gcc --version | head -1)"

rm -rf "$BUILD_DIR" && mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "[1/3] CMake configure..."
cmake \
    -DCMAKE_TOOLCHAIN_FILE="$REPO_DIR/toolchain-aarch64.cmake" \
    -DPORTMASTER=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DFINAL_ENABLED=ON \
    "$@" \
    "$REPO_DIR/Code"

echo "[2/3] Build..."
make -j"$(nproc)" Eld

echo "[3/3] Strip..."
mkdir -p "$(dirname "$OUTPUT")"
aarch64-linux-gnu-strip -o "$OUTPUT" "$BUILD_DIR/Projects/Eld/Eld"
chmod +x "$OUTPUT"
file "$OUTPUT"
