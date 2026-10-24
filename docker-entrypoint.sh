#!/bin/bash
# Runs inside the Docker image: cross-compiles Eldritch for aarch64 PortMaster devices.
# Extra arguments are passed to CMake (for example -DELD_NO_VBO=ON).

set -e

BUILD_DIR="/src/build-portmaster"
TOOLCHAIN="/src/toolchain-aarch64.cmake"
OUTPUT="/src/ports/eldritch/eldritch/eldritch.aarch64"

echo "============================================="
echo "  Eldritch -> PortMaster cross-compile (aarch64)"
echo "============================================="

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo ""
echo "[1/3] Running CMake configure..."
cmake \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
    -DPORTMASTER=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DFINAL_ENABLED=ON \
    "$@" \
    /src/Code

echo ""
echo "[2/3] Building ($(nproc) parallel jobs)..."
make -j"$(nproc)" Eld

echo ""
echo "[3/3] Stripping and copying binary..."
mkdir -p "$(dirname "$OUTPUT")"
aarch64-linux-gnu-strip -o "$OUTPUT" "$BUILD_DIR/Projects/Eld/Eld"
chmod +x "$OUTPUT"
file "$OUTPUT"

echo ""
echo "  Build successful: ports/eldritch/eldritch/eldritch.aarch64"
