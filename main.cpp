#include "common.hpp"
#include "game.cpp"
#include "menu.cpp"

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) return -1;

    gameLoop(window, renderer);
    close(window, renderer);
    return 0;
}
