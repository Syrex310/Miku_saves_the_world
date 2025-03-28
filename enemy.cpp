#include "common.hpp"
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
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (auto& enemy : enemies) {
        SDL_Rect rect = { enemy.x, enemy.y, enemy.width, enemy.height };
        SDL_RenderCopy(renderer, enemyTexture, NULL, &rect);
        //SDL_RenderFillRect(renderer, &rect);
    }
}
void spawnEnemy(int currentWave) {
    const int SAFE_ZONE_RADIUS = 200;
    bool tooClose;

    do {
        enemy.x = rand() % SCREEN_WIDTH;  
        enemy.y = rand() % SCREEN_HEIGHT;  

        int dx = enemy.x - player.x;
        int dy = enemy.y - player.y;
        float distance = sqrt(dx * dx + dy * dy);

        tooClose = (distance < SAFE_ZONE_RADIUS);
    } while (tooClose);

    enemy.width = 40;
    enemy.height = 40;
    enemy.speed = 3 + (0.2 * currentWave);
    enemy.health += (10 * currentWave);

    enemies.push_back(enemy);
}
void spawnBoss(){
    enemy.width += 60;
    enemy.height += 60;
    enemy.speed = 0;
    enemy.health = 10000;
    enemies.push_back(enemy);

    //change back to normal enemies
    enemy.width -= 60;
    enemy. height -= 60;
    enemy.speed = 5;
    enemy.health = 150;
}

void spawnWaves(int &currentWave, int maxWaves, bool &waveActive, Uint32 &lastWaveTime, int &enemiesSpawned, Uint32 &lastEnemySpawnTime) {
    bool bossActive = false;
    if (currentWave >= maxWaves){
        bossActive = false;
        gameState = WON;
        return;
    }

    Uint32 currentTime = SDL_GetTicks();
    
    int Time_interval=1000;
    if (enemies.empty() && !waveActive && (currentTime - lastWaveTime >= 5000)) {
        currentWave++;
        waveActive = true;
        lastWaveTime = currentTime;
        enemiesSpawned = 0;
        Time_interval-=50;
        if (currentWave == 10 && bossActive == false){
            bossActive = true;
            spawnBoss();
        }

        
        cout << "Wave " << currentWave << " started with " << 3*pow(currentWave,2) << " enemies!" << endl;
    }
    int totalEnemies = 3*pow(currentWave,2);
    if (waveActive && enemiesSpawned < totalEnemies && (currentTime - lastEnemySpawnTime >= Time_interval)){
        spawnEnemy(currentWave);
        enemiesSpawned+=1;
        lastEnemySpawnTime=currentTime;
    }

    if (enemiesSpawned >= totalEnemies && !enemies.empty()) {
        lastWaveTime=currentTime;
        waveActive = false;
    }
}
