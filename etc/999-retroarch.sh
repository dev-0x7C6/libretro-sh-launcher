#!/bin/sh

export HOME=/home/user
export DISPLAY=:0 

/usr/bin/retroarch -L /usr/lib/x86_64-linux-gnu/libretro/sh_launcher_libretro.so &

sleep 1

kill -12 $(pidof retroarch | cut -d' ' -f1) # there is two instance of retroarch, and I can't fix that :/
