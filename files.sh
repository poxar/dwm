#!/bin/sh
set -eu

if test "$ROFI_RETV" = "0"; then
  cd "$HOME"
  fd --type f '.*' Documents Downloads Pictures
  exit 0
fi

coproc (handlr open "$HOME/$@")
