# C++ Game Engine

A lightweight, from-scratch 2D game engine written in C++17.  
**Dependencies: SDL2** (everything else is stdlib).

## Files

| File | Description |
|------|-------------|
| `main.cpp` | Entry point |
| `Engine.h/cpp` | SDL2 window + renderer, game loop with delta time, virtual hooks |
| `Vec2.h` | 2D vector math (operators, normalize, dot, etc.) |
| `Input.h/cpp` | Singleton input system — translates SDL2 keyboard events to `Key` states |
| `ECS.h/cpp` | Entity Component System — create entities, attach/query typed components |
| `TileMap.h` | `TileMap` component + `TileMapSystem::render()` |
| `Game.h/cpp` | Demo game — player moves through a tiled room |

## Dependencies

Install SDL2:
```bash
# Ubuntu / Debian
sudo apt install libsdl2-dev

# macOS (Homebrew)
brew install sdl2

# Windows (MSYS2)
pacman -S mingw-w64-x86_64-SDL2
```

## Building

```bash
chmod +x build.sh
./build.sh
```

Requires `g++` with C++17 support and SDL2. Output binary is `./game_engine`.

## Running

```bash
./game_engine
```

**Controls:** WASD or arrow keys to move · ESC to quit

## Architecture

### Engine loop

`Engine` owns the SDL2 window and renderer. Override virtual hooks in game subclass:

```cpp
class MyGame : public Engine {
    void onInit()                    override { /* spawn entities, load map */ }
    void onEvent(const SDL_Event& e) override { /* handle raw SDL events    */ }
    void onUpdate(float dt)          override { /* move things              */ }
    void onRender()                  override { /* draw via getRenderer()   */ }
};
```

### ECS

Entities are plain `uint32_t` IDs. Components are structs that inherit `Component`.

```cpp
struct Health : Component { int hp = 100; };

auto& ecs  = ECS::get();
EntityID e = ecs.createEntity();
ecs.addComponent<Health>(e);
ecs.getComponent<Health>(e)->hp -= 10;
```

### TileMap

```cpp
// Create a 20×15 grid with 32×32 px tiles
auto& map = ecs.addComponent<TileMap>(mapEntity, 20, 15, 32, 32);
map.set(col, row, TileType::Wall);

// Render it (call from onRender)
TileMapSystem::render(getRenderer(), map);

// Solid-tile query for collision
if (map.isSolid(playerPos)) { /* blocked */ }
```

**TileTypes:** `Empty`, `Ground`, `Wall`, `Water`  
Add by extending the `TileType` enum and the `tileColor()` table in `TileMap.h`.

### Input

```cpp
if (Input::get().isKeyDown(Key::W)) { /* move up */ }
```

SDL2 keyboard events are processed automatically by `Engine` each frame.

### Vec2

```cpp
Vec2 a{3, 4};
Vec2 b = a.normalized();
float d = a.dot(b);
Vec2 c = Vec2::up() + Vec2::right();
```

## Roadmap

- [x] SDL2 window + real input events
- [x] Tilemap component + renderer
- [x] Sprite / texture renderer
- [ ] Audio system
- [ ] Scene manager
- [ ] AABB collision system
