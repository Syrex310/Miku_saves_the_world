#include "common.hpp"

void updatePlayerMovement(Player& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_A]) player.x -= player.speed;
    if (keystate[SDL_SCANCODE_D]) player.x += player.speed;
    if (keystate[SDL_SCANCODE_W]) player.y -= player.speed;
    if (keystate[SDL_SCANCODE_S]) player.y += player.speed;
}

void renderPlayer(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect = { player.x, player.y, player.width, player.height };
    SDL_RenderFillRect(renderer, &rect);
}
