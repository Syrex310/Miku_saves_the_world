#include "common.hpp"
#include "game.cpp"

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!init(window, renderer)) {
        return -1;
    }
    initializeGame();
    gameLoop(window, renderer);
    close(window, renderer);
    return 0;
}