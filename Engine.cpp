#include "Engine.h"
#include "Input.h"
#include <iostream>
#include <stdexcept>
#include <chrono>

Engine::Engine(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {}

Engine::~Engine()
{
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Engine::run()
{
    // --- SDL init ---
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_width, m_height,
        SDL_WINDOW_SHOWN
    );
    if (!m_window)
        throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());

    m_renderer = SDL_CreateRenderer(m_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer)
        throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());

    m_running = true;
    onInit();

    auto prev = std::chrono::high_resolution_clock::now();

    while (m_running)
    {
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - prev).count();
        prev = now;

        handleEvents();
        onUpdate(dt);

        // --- clear ---
        SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
        SDL_RenderClear(m_renderer);

        onRender();

        SDL_RenderPresent(m_renderer);
    }

    onShutdown();
}

void Engine::stop()
{
    m_running = false;
}

void Engine::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            stop();
            return;
        }
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
        {
            Input::get().processEvent(e);
        }
        onEvent(e);
    }
}
