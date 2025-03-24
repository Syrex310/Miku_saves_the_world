#include "common.hpp"

std::vector<Bullet> bullets;

void Bullet::move() {
    x += speedX;
    y += speedY;
}

void renderBullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    for (auto& bullet : bullets) {
        SDL_Rect rect = { static_cast<int>(bullet.x), static_cast<int>(bullet.y), 5, 5 }; 
        SDL_RenderFillRect(renderer, &rect);
        bullet.move();
    }
}
void updateBullets(vector<Bullet>& bullets) {
    for (auto& bullet : bullets) {
        bullet.x += bullet.speedX;
        bullet.y += bullet.speedY;
    }
}
void shootBullet(vector<Bullet>& bullets, Player& player) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);  

    
    float dx = mouseX - player.x;
    float dy = mouseY - player.y;
    float length = sqrt(dx * dx + dy * dy);  

    if (length == 0) return;

    
    dx /= length;
    dy /= length;

    Bullet bullet(player.x + player.width / 2, player.y + player.height / 2, mouseX, mouseY);
    bullet.x = player.x + player.width / 2; 
    bullet.y = player.y + player.height / 2;
    bullet.speedX = dx * 10;
    bullet.speedY = dy * 10;

    bullets.push_back(bullet);
}
void handleInput(SDL_Event& event, Player& player, bool& running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            bullets.push_back(Bullet(player.x + player.width / 2, player.y, mouseX, mouseY));
        }
    }
}

