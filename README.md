# C++ Game Engine

A lightweight, from-scratch 2D game engine written in C++17. No external dependencies

## Files

| File | Description |
|------|-------------|
| `main.cpp` | Entry point |
| `Engine.h/cpp` | Core game loop with delta time and virtual hooks |
| `Vec2.h` | 2D vector math (operators, normalize, dot product) |
| `Input.h/cpp` | Singleton input system with key state tracking |
| `ECS.h/cpp` | Entity Component System — create entities, attach/query typed components |
| `Game.h/cpp` | Your game layer — extend Engine and define your own logic here |

## Building

```bash
./build.sh
```

Requires `g++` with C++17 support. Output binary is `./game_engine`.

## Running

```bash
./game_engine
```

## Architecture

### Engine loop

`Engine` runs a fixed-rate loop (~60 FPS) and calls three virtual methods I override in my game class:

```cpp
class MyGame : public Engine
{
    void onInit()           override { /* spawn entities */ }
    void onUpdate(float dt) override { /* move things */    }
    void onRender()         override { /* draw things */    }
};
```

### ECS

Entities are just IDs. Components are plain structs that inherit from `Component`.

```cpp
// define a component
struct Health : Component
{
    int hp = 100;
};

// use it
auto& ecs   = ECS::get();
EntityID e  = ecs.createEntity();
ecs.addComponent<Health>(e);

Health* h = ecs.getComponent<Health>(e);
h->hp -= 10;
```

### Vec2

```cpp
Vec2 a{3, 4};
Vec2 b = a.normalized();   // {0.6, 0.8}
float d = a.dot(b);
Vec2 c = Vec2::up() + Vec2::right();
```

### Input

```cpp
if (Input::get().isKeyDown(Key::W))
{
    // move up
}
```

Keys must be pressed/released manually for now (no OS window integration yet). Planned for a future renderer pass.

## Roadmap

- [ ] SDL2 or SFML window + real input events
- [ ] Sprite renderer
- [ ] Audio system
- [ ] Scene manager
- [ ] Collision detection
