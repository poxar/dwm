#!/bin/sh
set -eu

export SSH_AUTH_SOCK="$XDG_RUNTIME_DIR/ssh-agent.socket"

# hidpi scaling
export GDK_SCALE=2
export QT_SCALE_FACTOR=2

# input devices
xinput set-prop 12 "libinput Tapping Enabled" 1
setxkbmap us altgr-intl ctrl:nocaps

# autostart some applications
keyd-application-mapper -d &
nextcloud --background &
telegram-desktop &
signal-desktop &
firefox &
alacritty &

# wallpaper
xwallpaper --zoom Pictures/Wallpapers/4k/coastline-mountain-pass-road-morning-daylight-scenery-6016x6016-3791.jpg &

# run dwm
picom &
"$HOME/Code/dwm/dwmstatus.sh" &
exec "$HOME/Code/dwm/dwm"
