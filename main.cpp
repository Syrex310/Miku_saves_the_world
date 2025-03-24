#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>

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

void handleInput(SDL_Event& event, bool& running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
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

void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {
    bool running = true;
    SDL_Event event;

    Player player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50, 5 };

    for (int i = 0; i < 5; i ++){
        spawnEnemy();
    }

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; 

    Uint32 frameStart;
    int frameTime;

    Uint32 lastSpawnTime = SDL_GetTicks();
    const int spawnInterval = 5000;

    while (running) {
        frameStart = SDL_GetTicks();

        handleInput(event, running);
        updatePlayerMovement(player);
        
        if (SDL_GetTicks() - lastSpawnTime > spawnInterval) {
            spawnEnemy();
            lastSpawnTime = SDL_GetTicks();
        }

        for (auto& enemy : enemies) {
            enemy.moveTowardPlayer(player);
        }
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        renderPlayer(renderer, player);
        renderEnemies(renderer);

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
