#include "common.hpp"
using namespace std;
vector<Enemy> enemies;

void Enemy::moveTowardPlayer(Player& player) {
    int dx = player.x - x;
    int dy = player.y - y;
    float length = sqrt(dx * dx + dy * dy);
    if (length != 0) {
        float normalizedX = dx / length;
        float normalizedY = dy / length;
        x += round((normalizedX) * speed);
        y += round((normalizedY) * speed);
    }
}

void renderEnemies(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (auto& enemy : enemies) {
        SDL_Rect rect = { enemy.x, enemy.y, enemy.width, enemy.height };
        SDL_RenderFillRect(renderer, &rect);
    }
}
void spawnEnemy() {
    Enemy enemy;
    enemy.x = rand() % SCREEN_WIDTH;
    enemy.y = rand() % SCREEN_HEIGHT;
    enemy.width = 40;
    enemy.height = 40;
    enemy.speed = 2;
    enemies.push_back(enemy);
}