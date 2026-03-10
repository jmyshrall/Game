#pragma once
#include <unordered_map>
#include <SDL2/SDL.h>

enum class Key
{
    W, A, S, D,
    Up, Down, Left, Right,
    Space, Escape,
    Unknown
};

class Input
{
public:
    static Input& get();

    // Called by Engine each frame to translate SDL events → Key states
    void processEvent(const SDL_Event& e);

    bool isKeyDown(Key k) const {
        auto it = m_keys.find(k);
        return it != m_keys.end() && it->second;
    }

    // Raw SDL scancode convenience
    bool isScanDown(SDL_Scancode sc) const {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        return state && state[sc];
    }

private:
    Input() = default;
    static Key sdlKeyToKey(SDL_Keycode kc);

    std::unordered_map<Key, bool> m_keys;
};
