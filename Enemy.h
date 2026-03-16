#pragma once
#include "ECS.h"
#include "Vec2.h"
#include <SDL2/SDL.h>

enum class EnemyState
{
    Idle,     // standing still, playing idle anim
    Patrol,   // walking between two waypoints
    Chase,    // locked onto player, moving toward them
    Attack,   // close enough to strike
};

struct Enemy : Component
{
    EnemyState state       = EnemyState::Patrol;

    // patrol waypoints (world-space positions)
    Vec2  waypointA        = {0, 0};
    Vec2  waypointB        = {0, 0};
    int   currentWaypoint  = 0;     // 0 = A, 1 = B

    float speed            = 80.f;  // pixels/sec while patrolling
    float chaseSpeed       = 140.f; // pixels/sec while chasing
    float detectionRange   = 180.f; // pixels — triggers Chase
    float attackRange      = 30.f;  // pixels — triggers Attack
    float loseSightRange   = 250.f; // pixels — returns to Patrol

    // attack cooldown
    float attackCooldown   = 0.8f;  // seconds between attacks
    float attackTimer      = 0.f;

    // tint colours per state (applied to the Sprite component if present)
    // defaults: grey patrol -> orange chase -> red attack
    SDL_Color patrolTint   = {180, 180, 200, 255};
    SDL_Color chaseTint    = {230, 140,  30, 255};
    SDL_Color attackTint   = {220,  40,  40, 255};

    Enemy() = default;
    Enemy(Vec2 a, Vec2 b,
          float spd = 80.f, float detect = 180.f)
        : waypointA(a), waypointB(b)
        , speed(spd), detectionRange(detect) {}
};

namespace EnemySystem
{
    void update(float dt, EntityID playerID, ECS& ecs);
} // namespace EnemySystem
