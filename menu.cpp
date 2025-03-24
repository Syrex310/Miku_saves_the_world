#include "common.hpp"

void handleMenuInput(SDL_Event& event, bool& running, GameState& gameState) {
    if (event.type == SDL_QUIT) running = false;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x > 700 && x < 900) {
            if (y > 300 && y < 360) gameState = GAME;
            if (y > 400 && y < 460) gameState = UPGRADES;
            if (y > 500 && y < 560) running = false;
        }
    }
}

void renderMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_RenderClear(renderer);
    renderText(renderer, font, "Start Game", 750, 310);
    renderText(renderer, font, "Upgrades", 750, 410);
    renderText(renderer, font, "Exit", 770, 510);
    SDL_RenderPresent(renderer);
}
