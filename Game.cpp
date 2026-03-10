#include "Game.h"
#include "Input.h"
#include <iostream>

// ---------------------------------------------------------------------------
// A simple demo map layout (0=empty, 1=ground, 2=wall, 3=water)
// 20 columns × 15 rows to fill a 640×480-ish area with 32px tiles
// ---------------------------------------------------------------------------
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
// clang-format on

// ---------------------------------------------------------------------------
Game::Game() : Engine("My Game Engine", 800, 640) {}

void Game::onInit()
{
    auto& ecs = ECS::get();

    // --- build tilemap ---
    m_mapEntity = ecs.createEntity();
    auto& map = ecs.addComponent<TileMap>(m_mapEntity, MAP_COLS, MAP_ROWS, 32, 32);

    for (int r = 0; r < MAP_ROWS; ++r)
        for (int c = 0; c < MAP_COLS; ++c)
            map.set(c, r, static_cast<TileType>(MAP_DATA[r][c]));

    // --- player ---
    m_player = ecs.createEntity();
    ecs.addComponent<Transform>(m_player, Vec2{96.f, 96.f});
    ecs.addComponent<Velocity>(m_player, Vec2{0, 0});
    ecs.addComponent<Tag>(m_player, "Player");

    std::cout << "[Game] Init complete. WASD or arrow keys to move. ESC to quit.\n";
}

void Game::onEvent(const SDL_Event& e)
{
    // Escape quits
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

    // --- build movement direction from input ---
    Vec2 dir{0, 0};
    if (input.isKeyDown(Key::W) || input.isKeyDown(Key::Up))    dir.y -= 1.f;
    if (input.isKeyDown(Key::S) || input.isKeyDown(Key::Down))  dir.y += 1.f;
    if (input.isKeyDown(Key::A) || input.isKeyDown(Key::Left))  dir.x -= 1.f;
    if (input.isKeyDown(Key::D) || input.isKeyDown(Key::Right)) dir.x += 1.f;

    float len = dir.length();
    if (len > 0.f) dir = dir / len;   // normalize diagonal movement

    vel->value = dir * m_playerSpeed;

    // --- simple wall collision against tilemap ---
    auto* map = ecs.getComponent<TileMap>(m_mapEntity);
    const float HALF = 12.f;   // player half-size for collision

    Vec2 next = xf->position + vel->value * dt;

    // check four corners of the player AABB
    auto blocked = [&](Vec2 pos) -> bool {
        if (!map) return false;
        return map->isSolid({pos.x - HALF, pos.y - HALF})
            || map->isSolid({pos.x + HALF, pos.y - HALF})
            || map->isSolid({pos.x - HALF, pos.y + HALF})
            || map->isSolid({pos.x + HALF, pos.y + HALF});
    };

    // resolve axes independently so player slides along walls
    Vec2 nextX = {next.x, xf->position.y};
    Vec2 nextY = {xf->position.x, next.y};

    if (!blocked(nextX)) xf->position.x = nextX.x;
    if (!blocked(nextY)) xf->position.y = nextY.y;
}

void Game::onRender()
{
    SDL_Renderer* rend = getRenderer();
    auto& ecs = ECS::get();

    // --- draw tilemap ---
    if (auto* map = ecs.getComponent<TileMap>(m_mapEntity))
        TileMapSystem::render(rend, *map);

    // --- draw player as a bright square ---
    auto* xf = ecs.getComponent<Transform>(m_player);
    if (xf)
    {
        const int HALF = 12;
        SDL_Rect rect {
            static_cast<int>(xf->position.x) - HALF,
            static_cast<int>(xf->position.y) - HALF,
            HALF * 2, HALF * 2
        };
        SDL_SetRenderDrawColor(rend, 230, 200, 60, 255);
        SDL_RenderFillRect(rend, &rect);

        // outline
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 200);
        SDL_RenderDrawRect(rend, &rect);
    }
}

void Game::onShutdown()
{
    std::cout << "[Game] Goodbye!\n";
}
