#!/bin/bash
clear
if ninja -j10; then
    "./bin/Linux - x86_64/Debug/game/game"
else
    echo BUILD CRASHED D:
fi
