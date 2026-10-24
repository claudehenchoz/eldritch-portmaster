## Notes

Thanks to David Pittman and [Minor Key Games](https://www.minorkeygames.com/) for creating Eldritch and releasing its source code under the zlib license.

Also thanks to [ptitSeb](https://github.com/ptitSeb/Eldritch) and rajdakin for the GLES2/ARM port of the engine, to [jardimdanificado](https://github.com/jardimdanificado/Eldritch-R36) for the R36S port, to slayer366 and kloristech for their help with it, and to claudehenchoz for the generic PortMaster packaging and single-stick controls.

The game data is not included. Buy Eldritch on [Steam](https://store.steampowered.com/app/252630/Eldritch/) or [itch.io](https://minorkeygames.itch.io/eldritch), then copy all `.cpk` files from the game folder (Windows, Linux or macOS version) into `ports/eldritch/`. The Mountains of Madness DLC is part of the current game files and is entered through a blue book in the hub. Tested with the current Steam release (build 412, shown as `Version:` in `log.txt`) on an RG 40XX V running muOS.

Saves and settings are kept in `ports/eldritch/conf/Eldritch Reanimated/`. If you used the old R36S release, move the contents of `ports/eldritch/userdata/` there. The game saves when you quit (including Select + Start) and when you go through a level exit.

On devices with one analog stick (for example the RG 40XXV), the stick looks around and the D-pad moves. Set `ELDRITCH_SINGLE_STICK=0` or `1` to override the automatic choice. To bind D-pad left/right in Options > Controls in this mode, hold Select while pressing the direction.

## Controls

Two analog sticks:

| Button | Action |
|--|--|
| Left Analog | Move |
| Right Analog | Look |
| D-Pad Left / Right | Lean |
| A | Jump |
| B | Crouch |
| X | Use / pick up |
| Y | Drop weapon |
| L1 | Run (toggle) |
| R1 | Cycle items |
| L2 | Use power |
| R2 | Attack |
| Start | Pause menu |
| Select + Start | Quit |

One analog stick:

| Button | Action |
|--|--|
| D-Pad | Move and strafe |
| Analog | Look |
| Select + D-Pad Left / Right | Lean |
| A, B, X, Y, L1, R1, L2, R2 | Same as above |
| Start | Pause menu |
| Select + Start | Quit |

## Compile

```shell
git clone https://github.com/claudehenchoz/eldritch-portmaster.git
cd eldritch-portmaster
./build_portmaster.sh    # Docker, Ubuntu 20.04: ports/eldritch/eldritch/eldritch.aarch64
```
