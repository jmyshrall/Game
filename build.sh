#!/bin/bash
set -e

cd "$(dirname "$0")"

CXX=${CXX:-g++}
FLAGS="-std=c++17 -O2 -Wall"
OUT="game_engine"

if command -v pkg-config &>/dev/null && pkg-config --exists sdl2 SDL2_image 2>/dev/null; then
    SDL_CFLAGS=$(pkg-config --cflags sdl2 SDL2_image)
    SDL_LIBS=$(pkg-config --libs sdl2 SDL2_image)
elif command -v pkg-config &>/dev/null && pkg-config --exists sdl2 2>/dev/null; then
    SDL_CFLAGS=$(pkg-config --cflags sdl2)
    SDL_LIBS="$(pkg-config --libs sdl2) -lSDL2_image"
else
    SDL_CFLAGS="-I/usr/include/SDL2"
    SDL_LIBS="-lSDL2 -lSDL2_image"
fi

echo "Building with SDL2 + SDL2_image..."

$CXX $FLAGS $SDL_CFLAGS \
    main.cpp            \
    Engine.cpp          \
    Input.cpp           \
    ECS.cpp             \
    Game.cpp            \
    Enemy.cpp           \
    Sprite.cpp          \
    TextureManager.cpp  \
    $SDL_LIBS           \
    -o $OUT

echo "Done — run with: ./$OUT"
