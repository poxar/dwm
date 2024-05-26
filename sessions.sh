#!/bin/sh
set -eu

if test "$ROFI_RETV" = "0"; then
  cd ~/Code
  fd -Is 'Session.vim' | xargs dirname
  exit 0
fi

coproc (alacritty --working-directory "$HOME/Code/$@" -e nvim -S Session.vim)
