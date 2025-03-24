#include "common.hpp"

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void updateCollisions(Player& player, std::vector<Enemy>& enemies, std::vector<Bullet>& bullets) {
    for (auto& enemy : enemies) {
        if (checkCollision(player.x, player.y, player.width, player.height, enemy.x, enemy.y, enemy.width, enemy.height)) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - player.lastDamageTime >= 1000) {
                player.health -= 1;
                player.lastDamageTime = currentTime;
            }
        }
    }

    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return e.health <= 0; }), enemies.end());
    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.x < 0 || b.x > SCREEN_WIDTH || b.y < 0 || b.y > SCREEN_HEIGHT; }), bullets.end());
}
