#pragma once
#include "ECS.h"
#include "Vec2.h"
#include "TextureManager.h"
#include <SDL2/SDL.h>
#include <string>

struct Sprite : Component
{
    SDL_Texture* texture  = nullptr;   // raw pointer — owned by TextureManager
    SDL_Rect     srcRect  = {0,0,0,0}; // source rect inside the sheet (0s = full)
    int          drawW    = 0;         // destination width  in pixels (0 = use src)
    int          drawH    = 0;         // destination height in pixels (0 = use src)

    SDL_Color    tint     = {255,255,255,255};  // colour-mod applied before draw
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    float        angle    = 0.f;       // degrees, clockwise

    bool         visible  = true;

    // convenience ctor — loads via TextureManager
    Sprite() = default;
    Sprite(SDL_Renderer* renderer, const std::string& path,
           int w = 0, int h = 0)
    {
        texture = TextureManager::get().load(renderer, path);
        drawW   = w;
        drawH   = h;
        // If no explicit size, fill srcRect from texture dimensions
        if (texture && (w == 0 || h == 0))
        {
            int tw, th;
            TextureManager::querySize(texture, tw, th);
            if (w == 0) drawW = tw;
            if (h == 0) drawH = th;
        }
        srcRect = {0, 0, drawW, drawH};
    }

    // set a sub-region of a sprite sheet
    void setSourceRect(int x, int y, int w, int h)
    {
        srcRect = {x, y, w, h};
        if (drawW == 0) drawW = w;
        if (drawH == 0) drawH = h;
    }
};

namespace SpriteSystem
{
    void render(SDL_Renderer* renderer, ECS& ecs);
} // namespace SpriteSystem
