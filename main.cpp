#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> 

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

using namespace std;
struct Player {
    int x, y;
    int width, height;
    int speed;
};

struct Enemy {
    int x, y;
    int width, height;
    int speed;

    void moveTowardPlayer(Player& player) {
        int dx = player.x - x;
        int dy = player.y - y;
    
        float length = sqrt(dx * dx + dy * dy);
        if (length != 0) {
            float normalizedX = dx / length;
            float normalizedY = dy / length;
    
            x += round(normalizedX * speed);
            y += round(normalizedY * speed);
        }
    }
    
};
vector<Enemy> enemies;

struct Bullet {
    int x, y;
    int speed = 10;
    int speedX, speedY;

    Bullet(int startX, int startY, int targetX, int targetY) {
        x = startX;
        y = startY;

        int dx = targetX - startX;
        int dy = targetY - startY;
        float length = sqrt(dx * dx + dy * dy);
        if (length != 0) {
            speedX = round(dx / length);
            speedY = round(dy / length);
        } else {
            speedX = 0;
            speedY = 0;
        }
    }

    void move() {
        x += speedX * speed;
        y += speedY * speed;
    }
};
vector<Bullet> bullets;


bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    return true;
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


void updatePlayerMovement(Player& player) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_A]) player.x -= player.speed;
    if (keystate[SDL_SCANCODE_D]) player.x += player.speed;
    if (keystate[SDL_SCANCODE_W]) player.y -= player.speed;
    if (keystate[SDL_SCANCODE_S]) player.y += player.speed;
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

void renderPlayer(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
    SDL_Rect rect = { player.x, player.y, player.width, player.height };
    SDL_RenderFillRect(renderer, &rect);
}

void renderEnemies(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (auto& enemy : enemies) {
        SDL_Rect rect = { enemy.x, enemy.y, enemy.width, enemy.height };
        SDL_RenderFillRect(renderer, &rect);
    }
}

void renderBullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    for (auto& bullet : bullets) {
        SDL_Rect rect = { bullet.x, bullet.y, 5, 5 }; 
        SDL_RenderFillRect(renderer, &rect);
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



void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {
    bool running = true;
    SDL_Event event;

    Player player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50, 3 };

    for (int i = 0; i < 5; i ++){
        spawnEnemy();
    }

    for (auto& bullet : bullets) {
        bullet.move();
    }    

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; 

    Uint32 frameStart;
    int frameTime;

    Uint32 lastSpawnTime = SDL_GetTicks();
    const int spawnInterval = 5000;

    while (running) {
        frameStart = SDL_GetTicks();

        handleInput(event, player, running);
        updatePlayerMovement(player);
        
        if (SDL_GetTicks() - lastSpawnTime > spawnInterval) {
            spawnEnemy();
            lastSpawnTime = SDL_GetTicks();
        }

        for (auto& enemy : enemies) {
            enemy.moveTowardPlayer(player);
        }
        
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) {
            return bullet.x < 0 || bullet.x > SCREEN_WIDTH || bullet.y < 0 || bullet.y > SCREEN_HEIGHT;
        }), bullets.end());        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);
        
        updateBullets(bullets);
        renderPlayer(renderer, player);
        renderEnemies(renderer);
        renderBullets(renderer);
        

        SDL_RenderPresent(renderer);

        // Frame rate control
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}


void close(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        return -1;
    }

    gameLoop(window, renderer);
    close(window, renderer);

    return 0;
}
