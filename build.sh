#!/bin/bash
set -e

cd "$(dirname "$0")"

CXX=${CXX:-g++}
FLAGS="-std=c++17 -O2 -Wall"
OUT="game_engine"

# detect SDL2 flags via pkg-config, fall back to sensible defaults
if command -v pkg-config &>/dev/null && pkg-config --exists sdl2; then
    SDL_FLAGS=$(pkg-config --cflags --libs sdl2)
else
    SDL_FLAGS="-I/usr/include/SDL2 -lSDL2"
fi

echo "Building with SDL2..."

$CXX $FLAGS \
    main.cpp   \
    Engine.cpp \
    Input.cpp  \
    ECS.cpp    \
    Game.cpp   \
    $SDL_FLAGS \
    -o $OUT

echo "done run with: ./$OUT"
