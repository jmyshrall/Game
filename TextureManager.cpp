#include "TextureManager.h"
#include <iostream>

TextureManager& TextureManager::get()
{
    static TextureManager instance;
    return instance;
}

SDL_Texture* TextureManager::load(SDL_Renderer* renderer, const std::string& path)
{
    // return cached copy if available
    auto it = m_cache.find(path);
    if (it != m_cache.end()) return it->second;

    // First load — initialise SDL_image for PNG+JPG if not already done
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
    {
        std::cerr << "[TextureManager] IMG_Init failed: " << IMG_GetError() << "\n";
        return nullptr;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        std::cerr << "[TextureManager] Failed to load '" << path << "': "
                  << IMG_GetError() << "\n";
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        std::cerr << "[TextureManager] SDL_CreateTextureFromSurface failed: "
                  << SDL_GetError() << "\n";
        return nullptr;
    }

    m_cache[path] = tex;
    std::cout << "[TextureManager] Loaded '" << path << "'\n";
    return tex;
}

void TextureManager::unload(const std::string& path)
{
    auto it = m_cache.find(path);
    if (it != m_cache.end())
    {
        SDL_DestroyTexture(it->second);
        m_cache.erase(it);
    }
}

void TextureManager::clear()
{
    for (auto& [path, tex] : m_cache)
        SDL_DestroyTexture(tex);
    m_cache.clear();
}

bool TextureManager::querySize(SDL_Texture* tex, int& w, int& h)
{
    if (!tex) return false;
    return SDL_QueryTexture(tex, nullptr, nullptr, &w, &h) == 0;
}
