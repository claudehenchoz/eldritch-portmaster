#!/bin/bash

XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}
if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
elif [ -d "$XDG_DATA_HOME/PortMaster/" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
else
  controlfolder="/roms/ports/PortMaster"
fi

source $controlfolder/control.txt
[ -f "${controlfolder}/mod_${CFW_NAME}.txt" ] && source "${controlfolder}/mod_${CFW_NAME}.txt"
get_controls

GAMEDIR=/$directory/ports/eldritch
CONFDIR="$GAMEDIR/conf"
BINARY=eldritch.${DEVICE_ARCH}

cd $GAMEDIR

> "$GAMEDIR/log.txt" && exec > >(tee "$GAMEDIR/log.txt") 2>&1

if [ ! -f "eldritch-base.cpk" ]; then
    pm_show_error "Copy the .cpk files from your Eldritch installation into ports/eldritch/." --title "Game data missing"
    exit 1
fi

# Saves and prefs.cfg go to conf/Eldritch Reanimated/
mkdir -p "$CONFDIR/Eldritch Reanimated"
PREFS="$CONFDIR/Eldritch Reanimated/prefs.cfg"
[ -f "$PREFS" ] || printf 'Fullscreen = true\nVSync = false\nFXAA = false\nUpscaleFullscreen = false\n' > "$PREFS"

export XDG_DATA_HOME="$CONFDIR"
export LD_LIBRARY_PATH="$GAMEDIR/libs.${DEVICE_ARCH}:$LD_LIBRARY_PATH"
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"

# One stick: the stick looks and the D-pad moves
[ -z "$ELDRITCH_SINGLE_STICK" ] && [ "$ANALOG_STICKS" = "1" ] && export ELDRITCH_SINGLE_STICK=1
echo "Analog sticks: ${ANALOG_STICKS:-unknown}, single-stick mode: ${ELDRITCH_SINGLE_STICK:-0}"

$GPTOKEYB "$BINARY" -c "./eldritch.gptk" &

pm_platform_helper "$GAMEDIR/$BINARY"

./$BINARY

pm_finish
