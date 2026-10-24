#!/bin/bash
# Checks that the PortMaster binary only depends on libraries every firmware
# ships, and on glibc / libstdc++ versions no newer than Ubuntu 20.04's.
# Usage: tools/check_binary.sh ports/eldritch/eldritch/eldritch.aarch64

set -e

BIN="${1:?usage: $0 <binary>}"
READELF="${READELF:-$(command -v aarch64-linux-gnu-readelf || command -v readelf)}"
OBJDUMP="${OBJDUMP:-$(command -v aarch64-linux-gnu-objdump || command -v objdump)}"

MAX_GLIBC="2.31"
MAX_GLIBCXX="3.4.28" # libstdc++ from GCC 10, as shipped by Ubuntu 20.04
ALLOWED='^(libc\.so\.6|libm\.so\.6|libdl\.so\.2|libpthread\.so\.0|librt\.so\.1|libstdc\+\+\.so\.6|libgcc_s\.so\.1|ld-linux-aarch64\.so\.1|libSDL2-2\.0\.so\.0|libGLESv2\.so\.2|libEGL\.so\.1)$'

fail=0

echo "NEEDED libraries:"
for lib in $("$READELF" -d "$BIN" | sed -n 's/.*(NEEDED).*\[\(.*\)\]/\1/p'); do
    if [[ "$lib" =~ $ALLOWED ]]; then
        echo "  ok   $lib"
    else
        echo "  BAD  $lib"
        fail=1
    fi
done

# Highest version of each symbol family, compared with sort -V
highest() {
    "$OBJDUMP" -T "$BIN" | grep -o "$1_[0-9][0-9.]*" | sed "s/^$1_//" | sort -uV | tail -1
}
newer_than() { [ "$(printf '%s\n%s\n' "$1" "$2" | sort -V | tail -1)" != "$2" ]; }

GLIBC=$(highest GLIBC)
GLIBCXX=$(highest GLIBCXX)
echo "Highest GLIBC_:   ${GLIBC:-none} (max $MAX_GLIBC)"
echo "Highest GLIBCXX_: ${GLIBCXX:-none} (max $MAX_GLIBCXX)"

if [ -n "$GLIBC" ] && newer_than "$GLIBC" "$MAX_GLIBC"; then
    echo "ERROR: needs glibc $GLIBC"
    fail=1
fi
if [ -n "$GLIBCXX" ] && newer_than "$GLIBCXX" "$MAX_GLIBCXX"; then
    echo "ERROR: needs GLIBCXX $GLIBCXX"
    fail=1
fi

[ "$fail" = 0 ] && echo "Binary checks passed."
exit "$fail"
