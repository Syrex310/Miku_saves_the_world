#include "common.hpp"

bool movingLeft = true;
void updatePlayerMovement(Player& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    int dx = 0, dy = 0;
    if (keystate[SDL_SCANCODE_A]) { dx = -1; movingLeft = true; }
    if (keystate[SDL_SCANCODE_D]) { dx = 1; movingLeft = false; }
    if (keystate[SDL_SCANCODE_W]) dy = -1;
    if (keystate[SDL_SCANCODE_S]) dy = 1;

    float length = sqrt(dx * dx + dy * dy);
    if (length != 0) {
        float normalizedX = dx / length;
        float normalizedY = dy / length;
        player.x += round(normalizedX * player.speed);
        player.y += round(normalizedY * player.speed);
    }
}


void renderPlayer(SDL_Renderer* renderer, Player& player) {
    /*
    SDL_Rect rect = { player.x, player.y, player.width, player.height };
    SDL_RenderCopy(renderer, playerTexture, NULL, &rect);
    */
    SDL_Rect play = {player.x, player.y, player.width, player.height};
    if (!movingLeft){
        SDL_RenderCopy(renderer, player1, NULL, &play);
    }
    else{
        SDL_RenderCopy(renderer, player2, NULL, &play);
    }

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