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
| `dlc1.cpk` present? | |
| Port build (release tag or commit) | |
| Single-stick mode (log line `single-stick mode: …`) | |

## Coverage wanted

Devices:

| Device | Why | Tested by / result |
|--|--|--|
| ANBERNIC RG 40XXV | Primary target: one stick, 640×480, Mali-G31 | |
| R36S | Regression check: two sticks, 640×480 | |
| 720×720 device (e.g. RG CubeXX) | Square screen, UI layout | |
| 1280×720 device | Higher resolution, performance | |
| 1024×768 device | 4:3 at a higher resolution | |
| Device with no analog stick | Unsupported for now (no way to look); confirm it at least boots | |

Firmware (PortMaster-New asks for all of these):

| Firmware | Display / driver | Tested by / result |
|--|--|--|
| ROCKNIX | Wayland/sway, Panfrost or libmali | |
| muOS | fbdev, libmali | |
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
| 37 | Mountains of Madness (DLC) is reachable, if `dlc1.cpk` is present | | |
