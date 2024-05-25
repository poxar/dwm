#!/bin/sh
set -eu

case "$*" in
  "Lock screen") coproc (xsecurelock) ;;
  "Quit") pkill dwm ;;
  "Poweroff") exec systemctl poweroff ;;
  "Reboot") exec systemctl reboot ;;
  "Hibernate") exec systemctl hibernate ;;
  "Suspend") exec systemctl suspend ;;
  *)
    echo "Lock screen"
    echo "Quit"
    echo "Poweroff"
    echo "Reboot"
    echo "Hibernate"
    echo "Suspend"
    ;;
esac
