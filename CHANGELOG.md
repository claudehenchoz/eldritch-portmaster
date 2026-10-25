# Changelog

## 0.1.1 (2026-10-25)

- Add `cover.png` (400×300, gameplay with the game's logo) and use it as the `gameinfo.xml` image, so frontends show a cover instead of the screenshot.

## 0.1.0 (2026-10-25)

First release as a generic PortMaster port, based on [jardimdanificado/Eldritch-R36](https://github.com/jardimdanificado/Eldritch-R36).

- Builds for all aarch64 PortMaster devices (`-DPORTMASTER=ON`; `-DR36S=ON` still works). The binary needs at most glibc 2.31 and links only libraries that PortMaster firmwares provide.
- Single-stick mode for devices with one analog stick, such as the RG 40XXV: the stick looks, the D-pad moves, and Select + D-pad left/right leans. It is turned on automatically when PortMaster reports exactly one stick.
- 24-bit depth buffer when the GPU supports it, which avoids z-fighting on distant geometry.
- Saves and settings go to `ports/eldritch/conf/Eldritch Reanimated/`.
- Clean shutdown on Select + Start (SIGTERM). The game and settings are saved first, and the rest of the teardown is bounded to a few seconds.
- The game always runs fullscreen. Links and "Go to Folder" no longer try to open a desktop program.
- Uses SDL2 for audio. OpenAL is no longer needed.
- Tested on an RG 40XX V running muOS with the Steam data (build 412). See [`TESTING.md`](TESTING.md).
