#!/bin/bash
clear
if make -o game; then
    "./bin/Linux - x86_64/Debug/game/game"
else
    echo BUILD CRASHED D:
fi
