#pragma once
#include "Engine.h"
#include "ECS.h"
#include "Vec2.h"

struct Transform : Component
{
    Vec2 position;
    float rotation = 0.f;
    Transform(Vec2 pos = Vec2::zero()) : position(pos) {}
};

struct Velocity : Component
{
    Vec2 value;
    Velocity(Vec2 v = Vec2::zero()) : value(v) {}
};

struct Tag : Component
{
    std::string name;
    Tag(const std::string& n) : name(n) {}
};

class Game : public Engine
{
public:
    Game();

protected:
    void onInit()           override;
    void onUpdate(float dt) override;
    void onRender()         override;
    void onShutdown()       override;

private:
    EntityID m_player  = 0;
    float    m_elapsed = 0.f;
};
