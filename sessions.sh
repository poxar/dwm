#!/bin/sh
set -eu

if test "$ROFI_RETV" = "0"; then
  find ~/Code -mindepth 1 -maxdepth 1 -type d
  exit 0
fi

if test -f "$@/Session.vim"; then
  coproc (alacritty --working-directory "$@" -e nvim -S Session.vim)
else
  coproc (alacritty --working-directory "$@" -e nvim "+Telescope find_files")
fi
