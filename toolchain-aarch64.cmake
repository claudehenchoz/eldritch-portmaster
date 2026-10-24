# CMake toolchain file for cross-compiling to aarch64-linux-gnu
# Targeting: aarch64 PortMaster handhelds (RK3326, H700, RK3566, ...)
#
# Usage: see build_portmaster.sh / build_portmaster_native.sh, or
#   cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-aarch64.cmake -DPORTMASTER=ON ../Code

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Toolchain binaries (provided by aarch64-linux-gnu-gcc in the Docker image)
set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_STRIP        aarch64-linux-gnu-strip)
set(CMAKE_AR           aarch64-linux-gnu-ar)
set(CMAKE_RANLIB       aarch64-linux-gnu-ranlib)

# Sysroot — set to / because the cross libs live directly at
# /usr/aarch64-linux-gnu/lib/ (not nested inside a second sysroot).
set(CMAKE_SYSROOT /)
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)

# Make CMake look in the sysroot for includes/libs
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# pkg-config: point to target's pkg-config data
set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_LIBDIR} "/usr/lib/aarch64-linux-gnu/pkgconfig:/usr/aarch64-linux-gnu/lib/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} "/")
