#include "common.hpp"
vector<Bullet> bullets;
void Bullet::move() {
    x += speedX;
    y += speedY;
}
void renderBullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    for (auto& bullet : bullets) {
        SDL_Rect bull = { (int)(bullet.x - 7), (int)(bullet.y - 7 + 12), 14,14 }; // adjusted to be in the character's hand(gun) position
        SDL_RenderCopy(renderer, bullet2, NULL, &bull);
    }
}
bool isShooting = false;
Uint32 lastShotTime = 0;
void handleGameInput(SDL_Event& event, bool& running, GameState& gameState) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
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
        if (mouseX > 1310 && mouseX < 1550 && mouseY >775 && mouseY < 860){
            restartGame();
        }
        if (mouseX > 550 && mouseX < 1055) {  
            if (mouseY > 350 && mouseY < 410) {
                gameState = GAME;
            }
            if (mouseY > 420 && mouseY < 480) {
                Mix_VolumeMusic(32);
                switchMusic(sBackground, "sound/Triple5 Here - Crystallogy.mp3");
                gameState = MENU;
            }
            if (mouseY > 490 && mouseY < 550) {
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
    if (gameState == WON){
        if (event.type = SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE){
            currency += 5000; //reward
            restartGame();
        }
    }
    //Check shooting state 
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) isShooting = true;
    else isShooting = false;
}


void updateShooting(Player& player) {
    Uint32 currentTime = SDL_GetTicks();
    int timeBetweenAttack = (currentTime - lastShotTime) * firerate;
    if (isShooting && timeBetweenAttack >= 250){ //shoot every 0.25 second modified by firerate of character above
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bullets.push_back(Bullet(player.x + player.width / 2, player.y + player.height / 2, mouseX, mouseY)); // point where the mouse is pointing
        lastShotTime = currentTime;
        Mix_PlayChannel(-1, sShot, 0);
    }
}
