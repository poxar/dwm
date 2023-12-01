#!/bin/sh
set -eu
 
selection=$(sk --with-nth=2 -d '\|' <<EOF | awk -F '|' '{ print $1 }'
slock|Lock screen
systemctl poweroff|Shut down / power off
pkill dwm|Log out
systemctl hibernate|Hibernate
EOF
)

if test -n "$selection"; then
  if sh -c "$selection"; then
    exit
  else
    sleep 60
  fi
fi
