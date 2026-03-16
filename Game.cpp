#include "Game.h"
#include "Input.h"
#include <cmath>
#include "TextureManager.h"
#include "Enemy.h"
#include <iostream>

static const int MAP_COLS = 25;
static const int MAP_ROWS = 20;

// clang-format off
static const uint8_t MAP_DATA[MAP_ROWS][MAP_COLS] = {
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,1,1,2},
    {2,1,1,2,0,1,1,1,1,1,1,1,1,1,1,1,0,0,2,1,1,1,1,1,2},
    {2,1,1,2,0,1,1,1,3,3,3,3,1,1,1,1,0,0,2,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,2,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,2},
    {2,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,2,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,2,1,1,1,1,1,1,2,0,2,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
};

Game::Game() : Engine("My Game Engine", 800, 640) {}

void Game::onInit()
{
    auto& ecs = ECS::get();

    m_mapEntity = ecs.createEntity();
    auto& map   = ecs.addComponent<TileMap>(m_mapEntity, MAP_COLS, MAP_ROWS, 32, 32);
    for (int r = 0; r < MAP_ROWS; ++r)
        for (int c = 0; c < MAP_COLS; ++c)
            map.set(c, r, static_cast<TileType>(MAP_DATA[r][c]));

    m_player = ecs.createEntity();
    ecs.addComponent<Transform>(m_player, Vec2{96.f, 96.f});
    ecs.addComponent<Velocity>(m_player, Vec2{0, 0});
    ecs.addComponent<Tag>(m_player, "Player");

    SDL_Renderer* rend = getRenderer();
    auto& tm = TextureManager::get();
    if (SDL_Texture* playerTex = tm.load(rend, "assets/player.png"))
    {
        auto& spr = ecs.addComponent<Sprite>(m_player);
        spr.texture  = playerTex;
        spr.drawW    = 24;
        spr.drawH    = 24;
        spr.srcRect  = {0, 0, 24, 24};
    }
    spawnEnemy({64,  192}, {640, 192}, 70.f, 180.f);
    spawnEnemy({64,  480}, {480, 480}, 60.f, 200.f);
    spawnEnemy({400, 300}, {400, 560}, 80.f, 150.f);  // vertical patrol

    std::cout << "[Game] Init complete. WASD / arrows to move, ESC to quit.\n";
}

void Game::spawnEnemy(Vec2 a, Vec2 b, float speed, float detect)
{
    auto& ecs = ECS::get();
    EntityID id = ecs.createEntity();

    ecs.addComponent<Transform>(id, a);
    ecs.addComponent<Velocity>(id, Vec2{0, 0});
    ecs.addComponent<Tag>(id, "Enemy");
    ecs.addComponent<Enemy>(id, a, b, speed, detect);

    SDL_Renderer* rend = getRenderer();
    if (SDL_Texture* tex = TextureManager::get().load(rend, "assets/enemy.png"))
    {
        auto& spr = ecs.addComponent<Sprite>(id);
        spr.texture = tex;
        spr.drawW   = 24;
        spr.drawH   = 24;
        spr.srcRect = {0, 0, 24, 24};
        spr.tint    = {180, 180, 200, 255};
    }
}

void Game::onEvent(const SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        stop();
}

void Game::onUpdate(float dt)
{
    auto& input = Input::get();
    auto& ecs   = ECS::get();

    auto* xf  = ecs.getComponent<Transform>(m_player);
    auto* vel = ecs.getComponent<Velocity>(m_player);
    if (!xf || !vel) return;

    Vec2 dir{0, 0};
    if (input.isKeyDown(Key::W) || input.isKeyDown(Key::Up))    dir.y -= 1.f;
    if (input.isKeyDown(Key::S) || input.isKeyDown(Key::Down))  dir.y += 1.f;
    if (input.isKeyDown(Key::A) || input.isKeyDown(Key::Left))  dir.x -= 1.f;
    if (input.isKeyDown(Key::D) || input.isKeyDown(Key::Right)) dir.x += 1.f;

    float len = dir.length();
    if (len > 0.f) dir = dir / len;
    vel->value = dir * m_playerSpeed;

    // flip player sprite
    if (auto* spr = ecs.getComponent<Sprite>(m_player))
    {
        if      (vel->value.x < 0) spr->flip = SDL_FLIP_HORIZONTAL;
        else if (vel->value.x > 0) spr->flip = SDL_FLIP_NONE;
    }

    auto* map = ecs.getComponent<TileMap>(m_mapEntity);
    const float HALF = 12.f;
    Vec2 next = xf->position + vel->value * dt;

    auto blocked = [&](Vec2 pos) -> bool {
        if (!map) return false;
        return map->isSolid({pos.x - HALF, pos.y - HALF})
            || map->isSolid({pos.x + HALF, pos.y - HALF})
            || map->isSolid({pos.x - HALF, pos.y + HALF})
            || map->isSolid({pos.x + HALF, pos.y + HALF});
    };

    if (!blocked({next.x, xf->position.y})) xf->position.x = next.x;
    if (!blocked({xf->position.x, next.y})) xf->position.y = next.y;

    EnemySystem::update(dt, m_player, ecs);
}

void Game::onRender()
{
    SDL_Renderer* rend = getRenderer();
    auto& ecs = ECS::get();

    // 1. Tilemap
    if (auto* map = ecs.getComponent<TileMap>(m_mapEntity))
        TileMapSystem::render(rend, *map);

    // 2. All sprites (enemies + player if they have a Sprite component)
    SpriteSystem::render(rend, ecs);

    // 3. Fallback solid-rect for entities WITHOUT a Sprite (player or enemies)
    for (auto& [id, comps] : ecs.entities())
    {
        if (comps.find(typeid(Sprite)) != comps.end()) continue; // has sprite, skip
        auto tit = comps.find(typeid(Transform));
        auto git = comps.find(typeid(Tag));
        if (tit == comps.end() || git == comps.end()) continue;

        auto* xf  = static_cast<Transform*>(tit->second.get());
        auto* tag = static_cast<Tag*>(git->second.get());

        const int HALF = 12;
        SDL_Rect rect {
            static_cast<int>(xf->position.x) - HALF,
            static_cast<int>(xf->position.y) - HALF,
            HALF * 2, HALF * 2
        };

        if (tag->name == "Player")
        {
            SDL_SetRenderDrawColor(rend, 230, 200, 60, 255);
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 255, 255, 255, 200);
            SDL_RenderDrawRect(rend, &rect);
        }
        else if (tag->name == "Enemy")
        {
            auto* enemy = ecs.getComponent<Enemy>(id);
            SDL_Color col = {180, 180, 200, 255};
            if (enemy)
            {
                switch (enemy->state)
                {
                    case EnemyState::Chase:   col = {230, 140,  30, 255}; break;
                    case EnemyState::Attack:  col = {220,  40,  40, 255}; break;
                    default: break;
                }
            }
            SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
            SDL_RenderFillRect(rend, &rect);
            SDL_SetRenderDrawColor(rend, 255, 255, 255, 120);
            SDL_RenderDrawRect(rend, &rect);

            // draw detection-range ring (thin circle approximation)
            if (enemy)
            {
                int cx = static_cast<int>(xf->position.x);
                int cy = static_cast<int>(xf->position.y);
                int r  = static_cast<int>(enemy->detectionRange);
                SDL_SetRenderDrawColor(rend, 255, 80, 80, 40);
                // Draw circle approximation for the detection ring
                for (int angle = 0; angle < 360; angle += 5)
                {
                    float rad = angle * 3.14159f / 180.f;
                    int x1 = cx + static_cast<int>(r * std::cos(rad));
                    int y1 = cy + static_cast<int>(r * std::sin(rad));
                    float rad2 = (angle + 5) * 3.14159f / 180.f;
                    int x2 = cx + static_cast<int>(r * std::cos(rad2));
                    int y2 = cy + static_cast<int>(r * std::sin(rad2));
                    SDL_RenderDrawLine(rend, x1, y1, x2, y2);
                }
            }
        }
    }
}

void Game::onShutdown()
{
    TextureManager::get().clear();
    std::cout << "[Game] Goodbye!\n";
}
