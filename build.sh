#!/bin/sh
mkdir -p output
gcc -O3 -flto carpet.c draw_utils_sdl.c -lSDL2 -lSDL2_image -o carpet.x
