#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> 
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
enum GameState { MENU, GAME, UPGRADES };
TTF_Font* font = TTF_OpenFont("OpenSans-Italic-VariableFont_wdth,wght.ttf", 24);


using namespace std;
struct Player {
    int x, y;
    int width, height;
    int speed;
    int health = 10;
    Uint32 lastDamageTime =0;
};

struct Enemy {
    int x, y;
    int width, height;
    int speed;
    int health = 50;

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
    float x, y;
    float speed = 10;
    float speedX, speedY;

    Bullet(int startX, int startY, int targetX, int targetY) {
        x = startX;
        y = startY;

        float dx = targetX - startX;
        float dy = targetY - startY;
        float length = sqrt(dx * dx + dy * dy);
        if (length != 0) {
            speedX = (dx / length) * speed;
            speedY = (dy / length) * speed;
        } else {
            speedX = 0;
            speedY = 0;
        }
    }

    void move() {
        x += speedX;
        y += speedY;
    }
};

vector<Bullet> bullets;

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void updateCollisions(Player& player, vector<Enemy>& enemies, vector<Bullet>& bullets) {
    for (auto& enemy : enemies) {
        if (checkCollision(player.x, player.y, player.width, player.height, enemy.x, enemy.y, enemy.width, enemy.height)) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - player.lastDamageTime >= 999) {
                player.health -= 1;
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
        SDL_Rect rect = { static_cast<int>(bullet.x), static_cast<int>(bullet.y), 5, 5 }; 
        SDL_RenderFillRect(renderer, &rect);
        bullet.move();
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

void renderHealth(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect healthBar = { 20, 20, player.health * 2, 10 };
    SDL_RenderFillRect(renderer, &healthBar);
}

void renderButton(SDL_Renderer* renderer, int x, int y, int w, int h, const char* text) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect button = { x, y, w, h };
    SDL_RenderFillRect(renderer, &button);
}

void handleMenuInput(SDL_Event& event, bool& running, GameState& gameState) {
    if (event.type == SDL_QUIT) {
        running = false;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX > 700 && mouseX < 900) {
            if (mouseY > 300 && mouseY < 360) gameState = GAME; 
            if (mouseY > 400 && mouseY < 460) gameState = UPGRADES; 
            if (mouseY > 500 && mouseY < 560) running = false; 
        }
    }
}

void renderMenu(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    renderButton(renderer, 700, 300, 200, 60, "Start Game");
    renderButton(renderer, 700, 400, 200, 60, "Upgrades");
    renderButton(renderer, 700, 500, 200, 60, "Exit");

    SDL_RenderPresent(renderer);
}


void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {
    bool running = true;
    SDL_Event event;
    GameState gameState = MENU;

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
        while (SDL_PollEvent(&event)) {
            if (gameState == MENU) {
                handleMenuInput(event, running, gameState);
            } else if (gameState == GAME) {
                handleInput(event, player, running);
            }
        }

        if (gameState == MENU) {
            renderMenu(renderer);
        } else if (gameState == GAME) {
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

            updateCollisions(player, enemies, bullets);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
            SDL_RenderClear(renderer);
            
            updateBullets(bullets);
            renderPlayer(renderer, player);
            renderEnemies(renderer);
            renderBullets(renderer);
            renderHealth(renderer, player);


            SDL_RenderPresent(renderer);

            // Frame rate control
            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
        }

        SDL_Delay(16);
        
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
