#include "common.hpp"
vector<Enemy> enemies;


void Enemy::moveTowardPlayer(Player& player) {
    int dx = (player.x + player.width / 2) - (x + width / 2);
    int dy = (player.y + player.height / 2) - (y + height / 2);
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
    enemy.speed = 2 + ((currentWave - 1) / 3); // + 1 speed / 3 waevs
    enemy.health = 50 + (25 * currentWave);

    enemies.push_back(enemy);
}
void spawnBoss(int currentWave){
    enemy.width += 160;
    enemy.height += 160;
    enemy.health *= (50 * currentWave);
    enemy.speed -= 1;
    enemies.push_back(enemy);

    //change back to normal enemies
    enemy.width -= 160;
    enemy.height -= 160;
    enemy.health = 50 + (25 * currentWave);
    enemy.speed += 1;
}



void spawnWaves(SDL_Renderer *renderer, int &currentWave, int maxWaves, bool &waveActive, Uint32 &lastWaveTime, int &enemiesSpawned, Uint32 &lastEnemySpawnTime) {
    bool bossActive = false;
    if (currentWave >= maxWaves){
        bossActive = false;
        gameState = WON;
        return;
    }

    Uint32 currentTime = SDL_GetTicks();
    
    int Time_interval=1000;
    if (enemies.empty() && !waveActive && (currentTime - lastWaveTime >= 5000)) {
        checkWave = false;
        currentWave++;
        waveActive = true;
        lastWaveTime = currentTime;
        enemiesSpawned = 0;
        Time_interval-=50;
        if (currentWave == 6) bossActive = false;
        if ((currentWave == 5 || currentWave == 10) && bossActive == false){
            //spawn boss moi 5 waves
            bossActive = true;
            spawnBoss(currentWave);
        }
        cout << "Wave " << currentWave << " started with " << 3*pow(currentWave,2) << " enemies!" << endl;
    }
    int totalEnemies = 3*pow(currentWave,2);
    if (waveActive && enemiesSpawned < totalEnemies && (currentTime - lastEnemySpawnTime >= Time_interval)){
        spawnEnemy(currentWave);
        enemiesSpawned+=1;
        lastEnemySpawnTime=currentTime;
        //renderText(renderer, font50, ("Enemies Left: "+ to_string(enemiesSpawned) + "/" + to_string((int)(3*pow(currentWave,2)))).c_str(), 800, 800, 800, 800, white);
    }

    if (enemiesSpawned >= totalEnemies && !enemies.empty()) {
        lastWaveTime=currentTime;
        waveActive = false;
    }
    else if (enemiesSpawned >= totalEnemies && enemies.empty()){
        checkWave = true;
    }
}
