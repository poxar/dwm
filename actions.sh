#!/bin/sh
set -eu

case "$*" in
  "Lock screen") coproc (slock) ;;
  "Quit") pkill dwm ;;
  "Poweroff") coproc systemctl poweroff ;;
  "Reboot") coproc systemctl reboot ;;
  "Hibernate") coproc systemctl hibernate ;;
  "Suspend") coproc systemctl suspend ;;
  *)
    echo "Lock screen"
    echo "Quit"
    echo "Poweroff"
    echo "Reboot"
    echo "Hibernate"
    echo "Suspend"
    ;;
esac
