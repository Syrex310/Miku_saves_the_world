#include "common.hpp"
#include "player.cpp"
#include "enemy.cpp"
#include "bullet.cpp"
#include "collision.cpp"
#include "rendering.cpp"

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    if (TTF_Init() == -1) return false;

    window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return renderer != nullptr;
}

void close(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {
    bool running = true;
    SDL_Event event;
    GameState gameState = MENU;
    TTF_Font* font = TTF_OpenFont("OpenSans.ttf", 24);
    Player player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50, 3 };

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (gameState == MENU) handleMenuInput(event, running, gameState);
            else if (gameState == GAME) handleInput(event, player, running);
        }

        if (gameState == MENU) renderMenu(renderer, font);
        else if (gameState == GAME) {
            updatePlayerMovement(player);
            for (auto& enemy : enemies) enemy.moveTowardPlayer(player);
            updateCollisions(player, enemies, bullets);

            SDL_RenderClear(renderer);
            renderPlayer(renderer, player);
            renderEnemies(renderer);
            renderBullets(renderer);
            renderHealth(renderer, player);
            SDL_RenderPresent(renderer);
        }
        SDL_Delay(16);
    }
}
