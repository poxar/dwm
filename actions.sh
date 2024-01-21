#!/bin/sh
set -eu

case "$*" in
  "Lock screen") exec xsecurelock ;;
  "Quit") pkill dwm ;;
  "Poweroff") exec systemctl poweroff ;;
  "Hibernate") exec systemctl hibernate ;;
  "Suspend") exec systemctl suspend ;;
  *)
    echo "Lock screen"
    echo "Quit"
    echo "Poweroff"
    echo "Hibernate"
    echo "Suspend"
    ;;
esac
