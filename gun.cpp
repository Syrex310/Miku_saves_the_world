#include "common.hpp"


double calculateAngle(int playerX, int playerY, int mouseX, int mouseY) {
    double deltaX = mouseX - playerX;
    double deltaY = mouseY - playerY;
    return atan2(deltaY, deltaX) * (180.0 / M_PI); //calculate to rad and convert to degree
}

void renderGun(SDL_Renderer* renderer, SDL_Texture* gunTexture, int playerX, int playerY) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    double angle = calculateAngle(playerX, playerY, mouseX, mouseY); // calculate angle from gun to mouse cursor

    SDL_Rect gunRect = {playerX, playerY, 36, 12};
    SDL_RenderCopyEx(renderer, gunTexture, NULL, &gunRect, angle, NULL, SDL_FLIP_NONE); // render with roration
}
