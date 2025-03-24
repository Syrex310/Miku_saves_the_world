#include "common.hpp"
using namespace std;
vector<Bullet> bullets;

void Bullet::move() {
    x += speedX;
    y += speedY;
}
void renderBullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    for (auto& bullet : bullets) {
        SDL_Rect rect = { static_cast<int>(bullet.x), static_cast<int>(bullet.y), 20, 20 }; 
        SDL_RenderFillRect(renderer, &rect);
    }
}
void updateBullets(vector<Bullet>& bullets) {
    for (auto& bullet : bullets) {
        bullet.x += bullet.speedX;
        bullet.y += bullet.speedY;
    }
}
bool isShooting = false;
Uint32 lastShotTime = 0;

void handleInput(SDL_Event& event, bool& running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            isShooting = true;
        }
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            isShooting = false;
        }
    }
}
void updateShooting(Player& player) {
    Uint32 currentTime = SDL_GetTicks();
    if (isShooting && currentTime > lastShotTime + 250){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bullets.push_back(Bullet(player.x + player.width / 2, player.y + player.height / 2, mouseX, mouseY));
        lastShotTime = currentTime;
    }
}
