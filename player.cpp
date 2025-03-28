#include "common.hpp"

void updatePlayerMovement(Player& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_A]) player.x -= player.speed;
    if (keystate[SDL_SCANCODE_D]) player.x += player.speed;
    if (keystate[SDL_SCANCODE_W]) player.y -= player.speed;
    if (keystate[SDL_SCANCODE_S]) player.y += player.speed;
}
void renderPlayer(SDL_Renderer* renderer, Player& player) {
    //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
    SDL_Rect rect = { player.x, player.y, player.width, player.height };
    SDL_RenderCopy(renderer, playerTexture, NULL, &rect);
    //SDL_RenderFillRect(renderer, &rect);
}
void renderRemainHealth(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect remainhealthBar = { 20, 20, 200 * player.health / player.maxhealth, 10 };
    SDL_RenderFillRect(renderer, &remainhealthBar);
}
void renderHealth(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect healthBar = { 20, 20, 200, 10 };
    SDL_RenderFillRect(renderer, &healthBar);
}
