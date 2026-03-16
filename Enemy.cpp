#include "Enemy.h"
#include "Game.h"
#include "Sprite.h"
#include <cmath>
#include <iostream>

namespace EnemySystem
{

static void applyTint(ECS& ecs, EntityID id, SDL_Color c)
{
    if (auto* spr = ecs.getComponent<Sprite>(id))
        spr->tint = c;
}

static void tickEnemy(float dt, EntityID enemyID, EntityID playerID, ECS& ecs)
{
    auto* xf     = ecs.getComponent<Transform>(enemyID);
    auto* vel    = ecs.getComponent<Velocity>(enemyID);
    auto* enemy  = ecs.getComponent<Enemy>(enemyID);
    auto* playerXf = ecs.getComponent<Transform>(playerID);

    if (!xf || !vel || !enemy || !playerXf) return;

    Vec2  ePos = xf->position;
    Vec2  pPos = playerXf->position;
    float dist = (pPos - ePos).length();

    switch (enemy->state)
    {
        case EnemyState::Idle:
        case EnemyState::Patrol:
            if (dist <= enemy->detectionRange)
            {
                enemy->state = EnemyState::Chase;
                std::cout << "[Enemy " << enemyID << "] Spotted player!\n";
            }
            break;

        case EnemyState::Chase:
            if (dist <= enemy->attackRange)
                enemy->state = EnemyState::Attack;
            else if (dist >= enemy->loseSightRange)
            {
                enemy->state = EnemyState::Patrol;
                std::cout << "[Enemy " << enemyID << "] Lost sight of player.\n";
            }
            break;

        case EnemyState::Attack:
            if (dist > enemy->attackRange)
                enemy->state = EnemyState::Chase;
            break;
    }

    vel->value = {0, 0};

    switch (enemy->state)
    {
        case EnemyState::Idle:
        case EnemyState::Patrol:
        {
            applyTint(ecs, enemyID, enemy->patrolTint);

            Vec2 target = (enemy->currentWaypoint == 0)
                            ? enemy->waypointA
                            : enemy->waypointB;

            Vec2 dir = target - ePos;
            float distToWp = dir.length();

            if (distToWp < 4.f)
            {
                // reached waypoint — switch to the other one
                enemy->currentWaypoint = 1 - enemy->currentWaypoint;
            }
            else
            {
                vel->value = (dir / distToWp) * enemy->speed;
            }

            // flip sprite based on horizontal direction
            if (auto* spr = ecs.getComponent<Sprite>(enemyID))
                spr->flip = (vel->value.x < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            break;
        }

        case EnemyState::Chase:
        {
            applyTint(ecs, enemyID, enemy->chaseTint);

            Vec2 dir = pPos - ePos;
            float d  = dir.length();
            if (d > 0.f) vel->value = (dir / d) * enemy->chaseSpeed;

            if (auto* spr = ecs.getComponent<Sprite>(enemyID))
                spr->flip = (vel->value.x < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            break;
        }

        case EnemyState::Attack:
        {
            applyTint(ecs, enemyID, enemy->attackTint);
            vel->value = {0, 0};

            enemy->attackTimer -= dt;
            if (enemy->attackTimer <= 0.f)
            {
                enemy->attackTimer = enemy->attackCooldown;
                // TODO: apply damage to player Health component when added
                std::cout << "[Enemy " << enemyID << "] Attacks player!\n";
            }
            break;
        }
    }

    // Apply velocity
    xf->position += vel->value * dt;
}

void update(float dt, EntityID playerID, ECS& ecs)
{
    for (auto& [id, comps] : ecs.entities())
    {
        if (comps.find(typeid(Enemy)) != comps.end())
            tickEnemy(dt, id, playerID, ecs);
    }
}

} // namespace EnemySystem
