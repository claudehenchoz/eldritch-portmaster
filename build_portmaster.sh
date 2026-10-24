#!/bin/bash
# Builds the aarch64 PortMaster binary in an Ubuntu 20.04 Docker image (glibc 2.31).
# Output: ports/eldritch/eldritch/eldritch.aarch64
# Extra arguments are passed to CMake, e.g. ./build_portmaster.sh -DELD_NO_VBO=ON

set -e

REPO_DIR="$(cd "$(dirname "$0")" && pwd)"
IMAGE_NAME="eldritch-portmaster"

if ! command -v docker &>/dev/null; then
    echo "ERROR: docker not found."
    exit 1
fi

echo "[1/2] Building Docker image '$IMAGE_NAME'..."
docker build -t "$IMAGE_NAME" "$REPO_DIR"

echo ""
echo "[2/2] Cross-compiling for aarch64..."
docker run --rm \
    --user "$(id -u):$(id -g)" \
    -v "$REPO_DIR":/src \
    "$IMAGE_NAME" "$@"

echo ""
echo "Binary: ports/eldritch/eldritch/eldritch.aarch64"
echo "Copy ports/eldritch/Eldritch.sh and ports/eldritch/eldritch/ to your device's ports folder,"
echo "then add the game's .cpk files to the eldritch/ folder."
