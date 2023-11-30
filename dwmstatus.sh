#!/bin/sh

trap "true" USR1

while true; do
  case "$(pamixer --get-mute)" in
    "true")  vol="MUT" ;;
    "false") vol="$(pamixer --get-volume)%" ;;
  esac

  date="$(date "+%a, %d. %b  %H:%M")"

  bat0="/sys/class/power_supply/BAT0"
  if test -f "$bat0/status"; then
    case "$(cat "$bat0/status")" in
      "Discharging") bat_sym="↓" ;;
      "Charging")    bat_sym="↑" ;;
      "Full")        bat_sym=""  ;;
      *)             bat_sym="!" ;;
    esac

    bat_full="$(cat "$bat0/energy_full")"
    bat_now="$(cat "$bat0/energy_now")"
    bat_perc="$(echo "$bat_now * 100 / $bat_full" | bc)"

    xsetroot -name " ${vol}  ${bat_sym}${bat_perc}%  ${date} "
  else
    xsetroot -name " ${vol}  ${date} "
  fi

  sleep 10 &
  wait $!
done
