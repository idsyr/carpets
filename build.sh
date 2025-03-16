#!/bin/sh
mkdir -p output
gcc -O2 carpet.c draw_utils_sdl.c -lSDL2 -lSDL2_image -o carpet.x
