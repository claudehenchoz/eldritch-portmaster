# Eldritch for PortMaster

A port of [Eldritch](https://www.minorkeygames.com/eldritch) by Minor Key Games to aarch64 Linux handhelds running [PortMaster](https://portmaster.games/) (ArkOS, dArkOS, muOS, Knulli, ROCKNIX, AmberELEC, ...). The primary targets are the ANBERNIC RG 40XXV (one analog stick) and the R36S (two sticks), both 640×480.

The game data is not included: you need the `.cpk` files from a purchased copy. See [`ports/eldritch/README.md`](ports/eldritch/README.md) for installation and controls, and [`TESTING.md`](TESTING.md) for the hardware test checklist.

Current version: **0.1.0** (see [`CHANGELOG.md`](CHANGELOG.md)).

## Lineage

1. Eldritch's source code, released by David Pittman / Minor Key Games under the zlib license (see `readme.txt` and `license.txt`)
2. [ptitSeb/Eldritch](https://github.com/ptitSeb/Eldritch): GLES2 renderer and ARM/OpenPandora port by ptitSeb and rajdakin
3. [jardimdanificado/Eldritch-R36](https://github.com/jardimdanificado/Eldritch-R36): R36S / ArkOS port
4. This repository: generic PortMaster build, single-stick controls and packaging for PortMaster-New

## Build options

| Option | Default | Effect |
|--|--|--|
| `-DPORTMASTER=ON` | OFF | PortMaster target: GLES2, system SDL2 headers, SoLoud SDL2 audio only, statically linked zlib, Cortex-A53 tuning when compiling for aarch64 |
| `-DR36S=ON` | OFF | Alias for `PORTMASTER`, kept for old build commands |
| `-DELD_NO_VBO=ON` | OFF | Draw without vertex buffer objects, to rule out VBO driver bugs ([ptitSeb/Eldritch#9](https://github.com/ptitSeb/Eldritch/issues/9)) |
| `-DELD_GL_DEBUG=ON` | OFF | Log `glGetError()` after GL calls in release builds (slow; for debugging only) |
| `-DFINAL_ENABLED=ON` | OFF | Shipping build without developer hotkeys; release builds use it |

Runtime environment variables:

| Variable | Effect |
|--|--|
| `ELDRITCH_SINGLE_STICK=1` | The analog stick looks, the D-pad moves, Select + D-pad left/right leans. The launcher sets it when PortMaster reports exactly one analog stick. |
| `ELDRITCH_USERDATA=<dir>` | Overrides the save and settings directory (PortMaster builds only). Otherwise `$XDG_DATA_HOME/Eldritch Reanimated/` is used. |

## Building

Release build (Docker, Ubuntu 20.04, glibc 2.31):

```shell
./build_portmaster.sh                  # -> ports/eldritch/eldritch/eldritch.aarch64
bash tools/check_binary.sh ports/eldritch/eldritch/eldritch.aarch64
bash tools/package.sh                  # -> eldritch.zip
```

Extra CMake options can be passed through, for example `./build_portmaster.sh -DELD_NO_VBO=ON`. `build_portmaster_native.sh` does the same with a locally installed `aarch64-linux-gnu` toolchain, without Docker.

Host compile check (x86_64, needs `libsdl2-dev`, `libgles2-mesa-dev` and `cmake`):

```shell
cmake -S Code -B build-host -DGLES=ON -DWITH_SDL2_STATIC=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build-host --target Eld -j
```

Single-stick unit test:

```shell
cmake -S Code -B build-test -DELD_BUILD_TESTS=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build-test --target SingleStickTest && ./build-test/UnitTests/SingleStickTest/SingleStickTest
```

GitHub Actions builds every push and pull request, checks the binary's library and glibc requirements, and uploads `eldritch.zip`. Tags starting with `v` (for example `v0.1.0`) publish a release.
