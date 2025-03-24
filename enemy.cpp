#include "common.hpp"

std::vector<Enemy> enemies;

void Enemy::moveTowardPlayer(Player& player) {
    int dx = player.x - x;
    int dy = player.y - y;
    float length = sqrt(dx * dx + dy * dy);
    if (length != 0) {
        x += round((dx / length) * speed);
        y += round((dy / length) * speed);
    }
}

void renderEnemies(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (auto& enemy : enemies) {
        SDL_Rect rect = { enemy.x, enemy.y, enemy.width, enemy.height };
        SDL_RenderFillRect(renderer, &rect);
    }
}
