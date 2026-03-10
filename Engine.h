#pragma once
#include <string>
#include <SDL2/SDL.h>

class Engine
{
public:
    Engine(const std::string& title, int width, int height);
    virtual ~Engine();

    void run();
    void stop();

    int          getWidth()    const { return m_width; }
    int          getHeight()   const { return m_height; }
    bool         isRunning()   const { return m_running; }
    SDL_Renderer* getRenderer() const { return m_renderer; }

protected:
    virtual void onInit()           {}
    virtual void onEvent(const SDL_Event&) {}
    virtual void onUpdate(float dt) {}
    virtual void onRender()         {}
    virtual void onShutdown()       {}

private:
    void handleEvents();

    std::string   m_title;
    int           m_width, m_height;
    bool          m_running  = false;

    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};
