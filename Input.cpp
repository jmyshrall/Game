#include "Input.h"

Input& Input::get()
{
    static Input instance;
    return instance;
}

void Input::processEvent(const SDL_Event& e)
{
    if (e.type != SDL_KEYDOWN && e.type != SDL_KEYUP) return;
    Key k = sdlKeyToKey(e.key.keysym.sym);
    m_keys[k] = (e.type == SDL_KEYDOWN);
}

Key Input::sdlKeyToKey(SDL_Keycode kc)
{
    switch (kc)
    {
        case SDLK_w:      return Key::W;
        case SDLK_a:      return Key::A;
        case SDLK_s:      return Key::S;
        case SDLK_d:      return Key::D;
        case SDLK_UP:     return Key::Up;
        case SDLK_DOWN:   return Key::Down;
        case SDLK_LEFT:   return Key::Left;
        case SDLK_RIGHT:  return Key::Right;
        case SDLK_SPACE:  return Key::Space;
        case SDLK_ESCAPE: return Key::Escape;
        default:          return Key::Unknown;
    }
}
