#!/bin/bash


name=${1:-"game"}

clear
path="./bin/Linux - x86_64/Debug/$name/$name"
echo "running $path"

if ninja -j10 engine $name; then
    "$path"
else
    echo BUILD CRASHED D:
fi
