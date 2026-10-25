# Hardware test checklist

The port was built and checked on a PC only. Everything below needs a real device. Copy this file, fill in one copy per device and firmware, and **attach `ports/eldritch/log.txt` to every report**.

Mark each item ✅ pass, ❌ fail or ➖ not tested, and add notes (what happened, FPS, screenshots).

## Test run

| Field | Value |
|--|--|
| Device | |
| Firmware and version | |
| GPU driver (Panfrost / libmali) | |
| Resolution | |
| Game data (store, OS, `Version:` line in log.txt) | |
| Port build (release tag or commit) | |
| Single-stick mode (log line `single-stick mode: …`) | |

## Coverage wanted

Devices:

| Device | Why | Tested by / result |
|--|--|--|
| ANBERNIC RG 40XXV | Primary target: one stick, 640×480, Mali-G31 | claudehenchoz: ✅ gameplay, rendering, performance, saving (see results below) |
| R36S | Regression check: two sticks, 640×480 | |
| 720×720 device (e.g. RG CubeXX) | Square screen, UI layout | |
| 1280×720 device | Higher resolution, performance | |
| 1024×768 device | 4:3 at a higher resolution | |
| Device with no analog stick | Unsupported for now (no way to look); confirm it at least boots | Miyoo Mini not usable: it is 32-bit ARM, and the port is aarch64 only |

Firmware (PortMaster-New asks for all of these):

| Firmware | Display / driver | Tested by / result |
|--|--|--|
| ROCKNIX | Wayland/sway, Panfrost or libmali | claudehenchoz, RG 40XXV: ✅ (see results below) |
| muOS | fbdev, libmali | claudehenchoz, RG 40XXV: ✅ (see results below) |
| dArkOS | KMSDRM, recent libmali | |
| Knulli | KMSDRM, libmali | |
| AmberELEC | legacy libmali | |
| ArkOS (optional) | KMSDRM | |

## Checks per run

### Start-up

| # | Check | Result | Notes |
|--|--|--|--|
| 1 | Without `.cpk` files, the launcher shows a "Game data missing" message and exits cleanly | | |
| 2 | With the data, the game reaches the title screen | | |
| 3 | log.txt shows the game data `Version:` line | | |
| 4 | log.txt shows `GLES depth buffer: 24-bit` or `16-bit` | | |
| 5 | No `Framebuffer incomplete` lines in log.txt | | |
| 6 | Audio and music play | | |

### Menus

| # | Check | Result | Notes |
|--|--|--|--|
| 7 | Navigate menus with the D-pad (two-stick mode) | | |
| 8 | Navigate menus with the left stick (two-stick mode) | | |
| 9 | Navigate menus with the D-pad (single-stick mode) | | |
| 10 | Navigate menus with the stick (single-stick mode) | | |
| 11 | Confirm with A / Start, back with B | | |
| 12 | Options > Controls (rebinding screen) works, especially in single-stick mode | | |
| 13 | UI is readable and correctly placed at this resolution | | |

### Gameplay

| # | Check | Two-stick | Single-stick | Notes |
|--|--|--|--|--|
| 14 | Move forward/back/strafe (left stick, or D-pad in single-stick) | | | |
| 15 | Diagonal movement is not faster than straight movement | | | |
| 16 | Look (right stick, or the only stick in single-stick) | | | |
| 17 | Jump (A), crouch (B) | | | |
| 18 | Frob / pick up (X), drop weapon (Y) | | | |
| 19 | Lean (D-pad left/right, or Select + D-pad left/right in single-stick) | | | |
| 20 | D-pad left/right alone strafes and does not lean (single-stick) | ➖ | | |
| 21 | Climbing ropes and ladders | | | |
| 22 | Attack (R2) and use power (L2) | | | |
| 23 | Run toggle (L1), cycle items (R1) | | | |

### Rendering

| # | Check | Result | Notes |
|--|--|--|--|
| 24 | The mirror in the hub shows the character | | |
| 25 | Enemies animate (no frozen or stretched meshes) | | |
| 26 | If 24 or 25 fails: retest with a `./build_portmaster.sh -DELD_NO_VBO=ON` build | | |
| 27 | No depth flicker (z-fighting) on distant geometry; note the depth line from check 4 | | |
| 28 | VSync off (default): tearing? smoothness? | | |
| 29 | VSync on (Options > Display): tearing? smoothness? input lag? | | |

### Performance

| # | Check | Result | Notes |
|--|--|--|--|
| 30 | Rough FPS in the hub | | |
| 31 | Rough FPS in a busy level (many enemies, particles) | | |

### Saving and exit

| # | Check | Result | Notes |
|--|--|--|--|
| 32 | Select + Start exits the game cleanly (back to the frontend, no hang) | | |
| 33 | After a relaunch, settings changed before exiting are kept | | |
| 34 | A run in progress resumes after Select + Start and relaunch | | |
| 35 | Progress is kept after going through a level exit and relaunching | | |
| 36 | Saves are in `ports/eldritch/conf/Eldritch Reanimated/` | | |
| 37 | Mountains of Madness (DLC, part of the current game files) loads from the blue book in the hub | | |
| 38 | Quit from the in-game menu returns to the frontend within a few seconds (no frozen screen; see the PC results below) | | |

## Results so far

### RG 40XXV, muOS (2026-10-24, claudehenchoz)

Port build: `main` at 2301832 (CI run 40088073551). Game data: Steam, build 412. Single-stick mode was enabled automatically (`Analog sticks: 1, single-stick mode: 1`).

| # | Check | Result | Notes |
|--|--|--|--|
| 2 | Reaches the title screen | ✅ | |
| 3 | `Version:` line | ✅ | `Version: 412` |
| 4 | Depth buffer line | ✅ | `GLES depth buffer: 24-bit (GL_OES_depth24)` |
| 5 | No `Framebuffer incomplete` | ✅ | |
| 6 | Audio | ✅ | `SoLoud backend: SDL2 (static)` |
| 32 | Select + Start exits cleanly | ✅ | log ends with `Termination signal received` … `Shutting down framework` |
| 33 | Settings kept after relaunch | ✅ | seeded `prefs.cfg` kept, rewritten on exit |
| 34 | Run resumes after Select + Start and relaunch | ✅ | `S/L: Restoring master file …` |
| 36 | Saves in `conf/Eldritch Reanimated/` | ✅ | `/mnt/sdcard/ports/eldritch/conf/Eldritch Reanimated/` |
| — | Shutting down works normally | ✅ | tester report |

Display: one 640×480 32-bit mode, fullscreen, no VSync.

Follow-up checks (2026-10-25, tester report):

| # | Check | Result | Notes |
|--|--|--|--|
| 21 | Climbing | ✅ | |
| 22 | Attack (R2), use power (L2) | ✅ | |
| 23 | Run toggle (L1), cycle items (R1) | ✅ | |
| 25 | Enemies animate | ✅ | enemies show, die when killed and can be looted |
| 26 | `ELD_NO_VBO` retest | ➖ | not needed: 25 passed with the default build |
| 27 | No depth flicker | ✅ | none seen, with the 24-bit depth buffer |
| 28 | VSync off (default) | ✅ | |
| 29 | VSync on | ✅ | works, but not needed |
| 30–31 | FPS | ✅ | fine |
| 35 | Level exit | ✅ | |

Not yet recorded on the device: missing-data message (1), menus and rebinding (7–13), movement, look and lean (14–20), the mirror (24), Mountains of Madness (37) and menu Quit (38). All of these passed on PC (see below). No other device is available to this tester. The Miyoo Mini can't stand in for the no-stick case, because it is 32-bit ARM.

### RG 40XXV, ROCKNIX (2026-10-25, claudehenchoz)

Same device, reflashed with ROCKNIX, with the same SD card and game data (Steam, build 412). The port folder was moved from muOS's `ports/eldritch/` to `roms/ports/eldritch/`, next to `Eldritch.sh`, with its saves in `conf/`.

| # | Check | Result | Notes |
|--|--|--|--|
| 2 | Reaches the title screen and runs | ✅ | tester report |
| 3 | `Version:` line | ✅ | `Version: 412` |
| 4 | Depth buffer line | ✅ | `GLES depth buffer: 24-bit (GL_OES_depth24)` |
| 5 | No `Framebuffer incomplete` | ✅ | |
| 6 | Audio backend | ✅ | `SoLoud backend: SDL2 (static)`; sound itself not reported |
| — | Single-stick mode on automatically | ✅ | `Analog sticks: 1, single-stick mode: 1` |
| 34 | Run resumes | ✅ | restored the master save and the hub and Water-1 world saves made under muOS |
| 36 | Saves in `conf/Eldritch Reanimated/` | ✅ | `/roms/ports/eldritch/conf/Eldritch Reanimated/`; save and `prefs.cfg` rewritten on exit |
| 38 | Quit from the in-game menu | ✅ | log ends with `Shutting down game` … `Shutting down framework`, with no termination signal, so the game exited by itself |

Display: sway (Wayland), one 640×480 mode, fullscreen, no VSync. Before the game window opened, PortMaster's sway helper tried 5 times to make `app_id=eldritch.aarch64` fullscreen and got `No matching node`. The engine requests fullscreen itself, and the tester confirmed the game ran fullscreen, so this is harmless. The log does not name the GPU driver (Panfrost or libmali).

### PC: x86_64 build under WSLg (2026-10-24, run by Claude)

These results come from the same engine code built for x86_64 (`-DPORTMASTER=ON`, plus a windowed non-PortMaster build for the resolution checks), running on Mesa llvmpipe (GLES 2) under WSLg with the Steam data (build 412). Input came from a virtual Xbox 360 pad created with uinput, so SDL's game controller path was exercised exactly as on a handheld. The launcher was run against a stub PortMaster `control.txt`. They do **not** replace device testing: the Mali driver, performance, VSync and the device's audio stack are untested here.

| # | Check | Two-stick | Single-stick | Notes |
|--|--|--|--|--|
| 1 | Missing data message, clean exit | ✅ | | `pm_show_error … --title Game data missing`, exit code 1, game not started |
| 2–5 | Title screen, `Version: 412`, 24-bit depth, no framebuffer errors | ✅ | ✅ | |
| 6 | Audio | ✅ | | `SoLoud backend: SDL2 (static)` with PulseAudio |
| 7 | Menus with the D-pad | ✅ | | |
| 8 | Menus with the left stick | ✅ | | |
| 9 | Menus with the D-pad | | ✅ | |
| 10 | Menus with the stick | | ✅ | the stick is reported as the right stick, which the menus accept |
| 11 | Confirm with A, back with B | ✅ | ✅ | |
| 12 | Controls rebinding screen | | ✅ | plain D-pad left/right is ignored when binding; Select + D-pad left binds the D-pad (shown with the D-pad icon) |
| 13 | UI at 640×480, 1024×768, 1280×720 | ✅ | | 720×720 not testable (no such mode under WSLg) |
| 14 | Move | ✅ | ✅ | left stick / D-pad up |
| 15 | Diagonal speed | | ✅ | unit length checked by `SingleStickTest`, not measured in game |
| 16 | Look | ✅ | ✅ | right stick / the stick |
| 17 | Jump (A), crouch (B) | | ✅ | crouch toggles, with the stealth vignette |
| 18 | Use (X) | | ✅ | opens the bank chest; B closes it |
| 19 | Lean | ✅ | ✅ | D-pad left / Select + D-pad left: view rolls, position unchanged on release |
| 20 | D-pad left/right alone strafes, no lean | ➖ | ✅ | |
| 22 | Attack (R2) | | ✅ | hit sparks on the chest; L2 not testable (no power equipped) |
| 24 | Mirror shows the animated character | ✅ | | also with an `ELD_NO_VBO=ON` build |
| 26 | `ELD_NO_VBO=ON` build renders the hub and the mirror | ✅ | | fallback works; whether it is *needed* is a Mali question |
| 32 | SIGTERM shuts down and saves | ✅ | ✅ | `Termination signal received` … master save written |
| 33, 34, 36 | Settings kept, run resumes, saves in `conf/` | ✅ | ✅ | through the launcher; `ANALOG_STICKS=1` → single-stick on, `2` → off |
| 37 | Mountains of Madness | ✅ | | "Strange Book" in the hub's blue fog → confirmation → snow level loads (`Rooms/MoM/Snow/…`) |
| 38 | Quit from the in-game menu | ✅ | | process gone 0.1 s after "Yes" |
| — | Two-stick devices unaffected | ✅ | | no single-stick log line without `ELDRITCH_SINGLE_STICK` |
| — | "Go to Folder…" does not run `xdg-open` | ✅ | | logs the user data path instead |

Not covered on PC: 21 (climbing), 23 (L1 run, R1 cycle), 25 (enemies in a level), 27 (depth flicker on Mali), 28–31 (VSync, FPS), 35 (level exit).

**Finding: shutdown could hang in audio teardown (now bounded).** In 2 of 4 shutdowns with WSLg's PulseAudio, the game saved and logged `Shutting down framework`, then hung forever: `SoLoud::deinit()` → `SDL_CloseAudio()` waits for SDL's PulseAudio thread, which is stuck in `pa_threaded_mainloop_wait()`. With SDL's dummy audio driver, 8 of 8 shutdowns were clean, and on the RG 40XXV shutdown works normally, so WSLg is the likely cause. As a safeguard, PortMaster builds now call `alarm(5)` right after the save and `prefs.cfg` are written: if the rest of the teardown hangs, SIGALRM ends the process. Verified by forcing a 30 s stall inside `SoLoud::deinit` under gdb: the process ended 5.3 s after SIGTERM (`terminated with signal SIGALRM`), with the save and `prefs.cfg` already on disk. Normal shutdowns are unaffected (5 of 5 exited in 0.4 s).

Note for PC testers: non-FINAL builds turn a failed assertion into `SIGINT` (a debugger breakpoint), which the PortMaster signal handler then handles as a clean shutdown. FINAL builds (all release builds) only log assertions.
