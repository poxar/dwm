#!/bin/sh
set -eu

cd "$HOME/Notes"

if test "$ROFI_RETV" = "0"; then
  fd --type f --exclude Archiv
  exit 0
fi

coproc (handlr open "$@")
