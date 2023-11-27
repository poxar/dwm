#!/bin/sh

while true; do
  case "$(pamixer --get-mute)" in
    "true")  vol="MUT" ;;
    "false") vol="$(pamixer --get-volume)%" ;;
  esac

  case "$(cat /sys/class/power_supply/BAT0/status)" in
    "Discharging") bat_sym="" ;;
    "Charging")    bat_sym="🠙" ;;
    *)             bat_sym="❗" ;;
  esac

  bat_full="$(cat /sys/class/power_supply/BAT0/energy_full)"
  bat_now="$(cat /sys/class/power_supply/BAT0/energy_now)"
  bat_perc="$(echo "$bat_now * 100 / $bat_full" | bc)"

  date="$(date "+%a, %d. %b  %H:%M")"

  xsetroot -name "🔉${vol}  ${bat_sym}${bat_perc}%  ${date} "
  sleep 1
done
