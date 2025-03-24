#include "common.hpp"

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void updateCollisions(Player& player, vector<Enemy>& enemies, vector<Bullet>& bullets) {
    for (auto& enemy : enemies) {
        if (checkCollision(player.x, player.y, player.width, player.height, enemy.x, enemy.y, enemy.width, enemy.height)) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - player.lastDamageTime >= 999) {
                player.health -= 10;
                player.lastDamageTime = currentTime;
            }
        }
    }
    
    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (checkCollision(static_cast<int>(bullet.x), static_cast<int>(bullet.y), 5, 5, enemy.x, enemy.y, enemy.width, enemy.height)) {
                enemy.health -= 25;
                bullet.x = -1000; 
            }
        }
    }
    
    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](Enemy& enemy) { return enemy.health <= 0; }), enemies.end());
    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) {
        return bullet.x < 0 || bullet.x > SCREEN_WIDTH || bullet.y < 0 || bullet.y > SCREEN_HEIGHT;
    }), bullets.end());
}
