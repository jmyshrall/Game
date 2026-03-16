#pragma once
#include "Engine.h"
#include "ECS.h"
#include "Vec2.h"
#include "TileMap.h"
#include "Sprite.h"
#include "Enemy.h"

struct Transform : Component
{
    Vec2  position;
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
    void onInit()                      override;
    void onEvent(const SDL_Event& e)   override;
    void onUpdate(float dt)            override;
    void onRender()                    override;
    void onShutdown()                  override;

private:
    void spawnEnemy(Vec2 waypointA, Vec2 waypointB,
                    float speed = 70.f, float detect = 180.f);

    EntityID m_player    = 0;
    EntityID m_mapEntity = 0;

    float m_playerSpeed  = 180.f;
};
