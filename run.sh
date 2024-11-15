#!/bin/bash
clear
if make -j4 -o game; then
    "./bin/Linux - x86_64/Debug/game/game"
else
    echo BUILD CRASHED D:
fi
