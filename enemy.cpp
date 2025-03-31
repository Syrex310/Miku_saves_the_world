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
        y += round((normalizedY) * speed); //make enemies move consistently
    }
}

void renderEnemies(SDL_Renderer* renderer) {
    for (auto& enemy : enemies) {
        SDL_Rect ene = { enemy.x, enemy.y, enemy.width, enemy.height };
        SDL_RenderCopy(renderer, enemyTexture, NULL, &ene);
    }
}
void spawnEnemy(int currentWave) {
    const int SAFE_ZONE_RADIUS = 350;
    bool tooClose;
    //respawn the enemy until it is far from the player (atleast 350 pixel)
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


    Uint32 currentTime = SDL_GetTicks();
    
    int Time_interval=1000; //time between enemy spawn (spawn rate)
    if (enemies.empty() && !waveActive && (currentTime - lastWaveTime >= 3000)) { // previous wave = done 5s ago + no enemies left
        checkWave = false;
        currentWave++;
        waveActive = true;
        lastWaveTime = currentTime;
        enemiesSpawned = 0;
        Time_interval-=75; // -0.075s enemy spawn rate per wave
        if (currentWave == 6) bossActive = false;
        if ((currentWave == 5 || currentWave == 10) && bossActive == false){
            //spawn boss every 5 waves
            bossActive = true;
            spawnBoss(currentWave);
        }
        cout << "Wave " << currentWave << " started with " << 2*pow(currentWave,2) << " enemies!" << endl;
    }
    int totalEnemies = 2*pow(currentWave,2);
    if (waveActive && enemiesSpawned < totalEnemies && (currentTime - lastEnemySpawnTime >= Time_interval)){
        if (currentWave >= 6){
            spawnEnemy(currentWave); spawnEnemy(currentWave);
            enemiesSpawned += 2;
            lastEnemySpawnTime = currentTime;
        }
        else{
            spawnEnemy(currentWave);
            enemiesSpawned+=1;
            lastEnemySpawnTime=currentTime;
        }
    }

    if (enemiesSpawned >= totalEnemies && !enemies.empty()) {
        lastWaveTime=currentTime;
        waveActive = false;
    }
    else if (enemiesSpawned >= totalEnemies && enemies.empty()){
        checkWave = true; // check to render text (current waves or enemies left that wave) in gameloop game.cpp
        if (currentWave == 10) {
            gameState = WON;
            bossActive = false;
        }
    }
}
