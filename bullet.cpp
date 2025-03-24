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
