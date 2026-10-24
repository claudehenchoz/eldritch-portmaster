# Cross-compilation image for Eldritch on aarch64 PortMaster devices.
# Base: Ubuntu 20.04 (Focal), glibc 2.31. This is deliberate: the binary must
# run on older firmwares such as ArkOS. Do NOT move to 22.04 (glibc 2.35).

FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# ---------------------------------------------------------------------------
# 1. Multiarch setup: amd64 packages from archive.ubuntu.com,
#                     arm64 packages from ports.ubuntu.com
# ---------------------------------------------------------------------------
RUN dpkg --add-architecture arm64 \
    && printf '\
deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal main restricted universe multiverse\n\
deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal-updates main restricted universe multiverse\n\
deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal-security main restricted universe multiverse\n\
deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports focal main restricted universe multiverse\n\
deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports focal-updates main restricted universe multiverse\n\
deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports focal-security main restricted universe multiverse\n\
' > /etc/apt/sources.list

# ---------------------------------------------------------------------------
# 2. Toolchain and target libraries (headers only; SDL2 and GLES come from the
#    device at runtime, zlib is linked statically)
# ---------------------------------------------------------------------------
RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc-aarch64-linux-gnu \
        g++-aarch64-linux-gnu \
        binutils-aarch64-linux-gnu \
        cmake \
        make \
        pkg-config \
        file \
        libsdl2-dev:arm64 \
        libgles2-mesa-dev:arm64 \
        zlib1g-dev:arm64 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY docker-entrypoint.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
