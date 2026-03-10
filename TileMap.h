#pragma once
#include "ECS.h"
#include "Vec2.h"
#include <vector>
#include <SDL2/SDL.h>

// ---------------------------------------------------------------------------
// Tile types — extend as needed
// ---------------------------------------------------------------------------
enum class TileType : uint8_t
{
    Empty  = 0,
    Ground = 1,
    Wall   = 2,
    Water  = 3,
};

// ---------------------------------------------------------------------------
// TileMap component
//   Stores a 2D grid of tile IDs plus rendering metadata.
// ---------------------------------------------------------------------------
struct TileMap : Component
{
    int tileW   = 32;   // tile width  in pixels
    int tileH   = 32;   // tile height in pixels
    int cols    = 0;
    int rows    = 0;

    Vec2 offset = {0, 0};   // world-space top-left origin

    std::vector<TileType> tiles;  // row-major: tiles[row * cols + col]

    TileMap() = default;
    TileMap(int cols, int rows, int tileW = 32, int tileH = 32)
        : tileW(tileW), tileH(tileH), cols(cols), rows(rows)
        , tiles(cols * rows, TileType::Empty) {}

    // -------------------------------------------------------------------
    // Accessors
    // -------------------------------------------------------------------
    TileType get(int col, int row) const
    {
        if (col < 0 || col >= cols || row < 0 || row >= rows) return TileType::Empty;
        return tiles[row * cols + col];
    }

    void set(int col, int row, TileType t)
    {
        if (col < 0 || col >= cols || row < 0 || row >= rows) return;
        tiles[row * cols + col] = t;
    }

    // World position → tile coordinate (returns false if out of bounds)
    bool worldToTile(Vec2 world, int& outCol, int& outRow) const
    {
        int c = static_cast<int>((world.x - offset.x) / tileW);
        int r = static_cast<int>((world.y - offset.y) / tileH);
        outCol = c; outRow = r;
        return c >= 0 && c < cols && r >= 0 && r < rows;
    }

    Vec2 tileToWorld(int col, int row) const
    {
        return { offset.x + col * tileW, offset.y + row * tileH };
    }

    // Convenience: is a world-space point inside a solid tile?
    bool isSolid(Vec2 world) const
    {
        int c, r;
        if (!worldToTile(world, c, r)) return false;
        TileType t = get(c, r);
        return t == TileType::Wall;
    }
};

// ---------------------------------------------------------------------------
// TileMapRenderer
//   A free function (or call it a "system") that draws a TileMap component.
//   Call it from your Game::onRender().
// ---------------------------------------------------------------------------
namespace TileMapSystem
{
    // Colour table — one SDL_Color per TileType
    inline SDL_Color tileColor(TileType t)
    {
        switch (t)
        {
            case TileType::Ground: return { 80, 140,  60, 255 };  // green-ish
            case TileType::Wall:   return { 90,  90,  90, 255 };  // grey
            case TileType::Water:  return { 50, 100, 200, 255 };  // blue
            default:               return {  0,   0,   0,   0 };  // transparent
        }
    }

    inline void render(SDL_Renderer* renderer, const TileMap& map)
    {
        for (int r = 0; r < map.rows; ++r)
        {
            for (int c = 0; c < map.cols; ++c)
            {
                TileType t = map.get(c, r);
                if (t == TileType::Empty) continue;

                SDL_Color col = tileColor(t);
                SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);

                SDL_Rect rect {
                    static_cast<int>(map.offset.x) + c * map.tileW,
                    static_cast<int>(map.offset.y) + r * map.tileH,
                    map.tileW,
                    map.tileH
                };
                SDL_RenderFillRect(renderer, &rect);

                // thin dark border
                SDL_SetRenderDrawColor(renderer, 20, 20, 20, 120);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
} // namespace TileMapSystem
