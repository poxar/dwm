#!/bin/sh

trap "true" USR1

bat_symbol() {
  bat_perc="$1"

  if test "$bat_perc" -ge 90; then
    echo " "
  elif test "$bat_perc" -ge 75; then
    echo " "
  elif test "$bat_perc" -ge 50; then
    echo " "
  elif test "$bat_perc" -ge 25; then
    echo " "
  else
    echo " "
  fi
}

while true; do
  case "$(pamixer --get-mute)" in
  "true") vol="MUT" ;;
  "false") vol="$(pamixer --get-volume)%" ;;
  esac
  vol="󰕾 $vol"

  case "$(pamixer --default-source --get-mute)" in
  "true") mic="" ;;
  "false") mic="REC " ;;
  esac

  date="󰨲 $(date "+%a, %d. %b  %H:%M")"

  bat0="/sys/class/power_supply/BAT0"
  if test -f "$bat0/status"; then
    bat_full="$(cat "$bat0/energy_full")"
    bat_now="$(cat "$bat0/energy_now")"
    bat_perc="$(echo "$bat_now * 100 / $bat_full" | bc)"

    case "$(cat "$bat0/status")" in
    "Discharging") bat_stat=$(bat_symbol "$bat_perc") ;;
    "Charging") bat_stat=" $(bat_symbol "$bat_perc")" ;;
    "Not Charging") bat_stat="!" ;;
    "Full") bat_stat="" ;;
    *) bat_stat="? " ;;
    esac

    xsetroot -name " ${mic}${vol}  ${bat_stat}  ${date} "
  else
    xsetroot -name " ${mic}${vol}  ${date} "
  fi

  sleep 10 &
  wait $!
done
