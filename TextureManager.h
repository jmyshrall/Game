#pragma once
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager
{
public:
    static TextureManager& get();

    // returns a cached SDL_Texture* (loads on first call).
    // returns nullptr on failure and prints to stderr.
    SDL_Texture* load(SDL_Renderer* renderer, const std::string& path);

    // explicitly drop a cached texture (frees GPU memory)
    void unload(const std::string& path);

    // free everything — called automatically on destruction
    void clear();

    // query width/height of a loaded texture
    static bool querySize(SDL_Texture* tex, int& w, int& h);

private:
    TextureManager() = default;
    ~TextureManager() { clear(); }
    TextureManager(const TextureManager&)            = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::unordered_map<std::string, SDL_Texture*> m_cache;
};
