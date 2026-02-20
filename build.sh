#!/bin/bash
set -e

cd "$(dirname "$0")"

CXX=${CXX:-g++}
FLAGS="-std=c++17 -O2 -Wall"
OUT="game_engine"

echo "building"

$CXX $FLAGS \
    main.cpp \
    Engine.cpp \
    Input.cpp \
    ECS.cpp \
    Game.cpp \
    -o $OUT

echo "done, run with: ./$OUT"
