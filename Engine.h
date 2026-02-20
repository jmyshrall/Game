#pragma once
#include <string>

class Engine
{
public:
    Engine(const std::string& title, int width, int height);
    ~Engine();

    void run();
    void stop();

    int   getWidth()  const { return m_width; }
    int   getHeight() const { return m_height; }
    bool  isRunning() const { return m_running; }

protected:
    virtual void onInit()   {}
    virtual void onUpdate(float dt) {}
    virtual void onRender() {}
    virtual void onShutdown() {}

private:
    std::string m_title;
    int   m_width, m_height;
    bool  m_running = false;
};
