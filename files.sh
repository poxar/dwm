#!/bin/sh
set -eu

if test "$ROFI_RETV" = "0"; then
  cd "$HOME"
  find Documents Downloads Pictures \
    -not -path '*/.*' \
    -type f \
    -printf "%T@\t%p\n" \
    | sort -r -n \
    | awk -F '\t' '{ print $2 }'
  exit 0
fi

coproc (handlr open "$HOME/$@")
