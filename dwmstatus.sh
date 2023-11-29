#!/bin/sh

trap "true" USR1

while true; do
  case "$(pamixer --get-mute)" in
    "true")  vol="MUT" ;;
    "false") vol="$(pamixer --get-volume)%" ;;
  esac

  date="$(date "+%a, %d. %b  %H:%M")"

  if test hostname = "tauron"; then
    case "$(cat /sys/class/power_supply/BAT0/status)" in
      "Discharging") bat_sym="v" ;;
      "Charging")    bat_sym="^" ;;
      "Full")        bat_sym=""  ;;
      *)             bat_sym="!" ;;
    esac

    bat_full="$(cat /sys/class/power_supply/BAT0/energy_full)"
    bat_now="$(cat /sys/class/power_supply/BAT0/energy_now)"
    bat_perc="$(echo "$bat_now * 100 / $bat_full" | bc)"

    xsetroot -name " ${vol}  ${bat_sym}${bat_perc}%  ${date} "
  else
    xsetroot -name " ${vol}  ${date} "
  fi

  sleep 10 &
  wait $!
done
