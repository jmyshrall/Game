#include "Engine.h"
#include <iostream>
#include <chrono>
#include <thread>

Engine::Engine(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {}

Engine::~Engine() {}

void Engine::run()
{
    m_running = true;
    onInit();

    std::cout << "[Engine] Started: " << m_title
              << " (" << m_width << "x" << m_height << ")\n";

    auto prev = std::chrono::high_resolution_clock::now();

    while (m_running)
    {
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - prev).count();
        prev = now;

        onUpdate(dt);
        onRender();

        // cap to ~60 FPS in headless mode
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    onShutdown();
    std::cout << "[Engine] Shut down.\n";
}

void Engine::stop()
{
    m_running = false;
}
