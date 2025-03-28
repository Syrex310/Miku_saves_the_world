#include "common.hpp"

void updatePlayerMovement(Player& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    int dx = 0, dy = 0;
    if (keystate[SDL_SCANCODE_A]) dx = -1;
    if (keystate[SDL_SCANCODE_D]) dx = 1;
    if (keystate[SDL_SCANCODE_W]) dy = -1;
    if (keystate[SDL_SCANCODE_S]) dy = 1;
    if (dx != 0 && dy != 0) {
        float normalizationFactor = 1.0f / sqrt(2);
        player.x += player.speed * dx * normalizationFactor;
        player.y += player.speed * dy * normalizationFactor;
    } else {
        player.x += player.speed * dx;
        player.y += player.speed * dy;
    }
}

void renderPlayer(SDL_Renderer* renderer, Player& player) {
    //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
    SDL_Rect rect = { player.x, player.y, player.width, player.height };
    SDL_RenderCopy(renderer, playerTexture, NULL, &rect);
    //SDL_RenderFillRect(renderer, &rect);
}
void renderRemainHealth(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect remainhealthBar = { 0, 15, 493 * player.health / player.maxhealth, 75 };
    SDL_RenderFillRect(renderer, &remainhealthBar);
}
void renderHealth(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect healthBar = { 0, 15, 493, 75 };
    SDL_RenderFillRect(renderer, &healthBar);
}
void renderIngame(SDL_Renderer *renderer){
    SDL_Rect rect = {0, 0, 1600, 900};
    SDL_RenderCopy(renderer, ingame, NULL, &rect);
}