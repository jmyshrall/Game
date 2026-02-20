#include "Game.h"
#include <iostream>

Game::Game() : Engine("My Game Engine", 1280, 720) {}

void Game::onInit()
{
    auto& ecs = ECS::get();

    m_player = ecs.createEntity();
    ecs.addComponent<Transform>(m_player, Vec2{640, 360});
    ecs.addComponent<Velocity>(m_player, Vec2{50, 0});
    ecs.addComponent<Tag>(m_player, "Player");

    std::cout << "[Game] Player entity created (id=" << m_player << ")\n";
}

void Game::onUpdate(float dt)
{
    m_elapsed += dt;

    auto& ecs = ECS::get();
    auto* xf  = ecs.getComponent<Transform>(m_player);
    auto* vel = ecs.getComponent<Velocity>(m_player);

    if (xf && vel)
    {
        xf->position += vel->value * dt;

        if (xf->position.x > getWidth()  || xf->position.x < 0) vel->value.x *= -1;
        if (xf->position.y > getHeight() || xf->position.y < 0) vel->value.y *= -1;
    }

    if (m_elapsed >= 3.f) stop();
}

void Game::onRender()
{
    auto* xf = ECS::get().getComponent<Transform>(m_player);
    if (xf)
    {
        std::cout << "[Render] Player @ ("
                  << xf->position.x << ", " << xf->position.y << ")\n";
    }
}

void Game::onShutdown()
{
    std::cout << "[Game] goodbye\n";
}
