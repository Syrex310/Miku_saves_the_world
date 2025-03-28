#include "common.hpp"
vector<Bullet> bullets;
void Bullet::move() {
    x += speedX;
    y += speedY;
}
void renderBullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    for (auto& bullet : bullets) {
        SDL_Rect bull = { static_cast<int>(bullet.x), static_cast<int>(bullet.y), 15,15 };
        SDL_RenderCopy(renderer, bullet2, NULL, &bull);
    }
}
bool isShooting = false;
Uint32 lastShotTime = 0;
void handleGameInput(SDL_Event& event, bool& running, GameState& gameState) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int buttonWidth = 200;
    int buttonHeight = 50;
    int centerX = (1600 - buttonWidth) / 2;
    int centerY = (900 - buttonHeight) / 2;
    int startY = 300;
    int spacing = 100;

    if (event.type == SDL_QUIT) {
        saveGame();
        running = false;
    }
    if (event.type == SDL_KEYDOWN  && event.key.repeat == 0) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            //cout<<"Game resumed / Game Paused";
            if (gameState == GAME) {
                saveGame();
                gameState = PAUSED;
            } 
            else if (gameState == PAUSED) {
                gameState = GAME;
            }
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN && gameState == PAUSED) {
        if (mouseX > centerX && mouseX < centerX + 300) {  
            if (mouseY > centerY && mouseY < centerY + 60) {
                gameState = GAME;
            }
            if (mouseY > centerY + 80 && mouseY < centerY + 140) {
                gameState = MENU;
            }
            if (mouseY > centerY + 160 && mouseY < centerY + 220) {
                saveGame();
                running = false;
            }
        }
    }
    if (gameState == DEAD){
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r){
            restartGame();
        }
    }
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) isShooting = true;
    else isShooting = false;
}


void updateShooting(Player& player) {
    Uint32 currentTime = SDL_GetTicks();
    int timeBetweenAttack = (currentTime - lastShotTime) * firerate;
    if (isShooting && timeBetweenAttack >= 250){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bullets.push_back(Bullet(player.x + player.width / 2, player.y + player.height / 2, mouseX, mouseY));
        lastShotTime = currentTime;
    }
}
