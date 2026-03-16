// SDL_MAIN_HANDLED must be defined before any SDL header is pulled in.
// It tells SDL2 not to redefine main→SDL_main, so the standard C++
// entry point is used directly. SDL_SetMainReady() informs the library
// that platform init (which SDL_main normally does) is our responsibility
// — SDL_Init() inside Engine::run() handles it.
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Game.h"

int main()
{
    SDL_SetMainReady();
    Game game;
    game.run();
    return 0;
}
