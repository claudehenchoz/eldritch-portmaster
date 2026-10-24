#!/bin/bash
# Packages eldritch.zip in the PortMaster user-install layout:
#   Eldritch.sh
#   eldritch/  (binary, gptk, licenses, README.md, port.json, gameinfo.xml, screenshot)
# Run after build_portmaster.sh. Usage: tools/package.sh [output.zip]

set -e

REPO_DIR="$(cd "$(dirname "$0")/.." && pwd)"
PORT_SRC="$REPO_DIR/ports/eldritch"
OUT="$(realpath -m "${1:-$REPO_DIR/eldritch.zip}")"
STAGE="$(mktemp -d)"
trap 'rm -rf "$STAGE"' EXIT

if [ ! -f "$PORT_SRC/eldritch/eldritch.aarch64" ]; then
    echo "ERROR: ports/eldritch/eldritch/eldritch.aarch64 missing; run build_portmaster.sh first."
    exit 1
fi

cp "$PORT_SRC/Eldritch.sh" "$STAGE/"
cp -r "$PORT_SRC/eldritch" "$STAGE/eldritch"
for f in README.md port.json gameinfo.xml screenshot.png; do
    [ -f "$PORT_SRC/$f" ] && cp "$PORT_SRC/$f" "$STAGE/eldritch/"
done
chmod +x "$STAGE/eldritch/eldritch.aarch64"

rm -f "$OUT"
(cd "$STAGE" && zip -r -X "$OUT" Eldritch.sh eldritch)
echo "Created $OUT"
