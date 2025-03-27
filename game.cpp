#include "common.hpp"
#include "save.cpp"
#include "enemy.cpp"
#include "player.cpp"
#include "collision.cpp"
#include "bullet.cpp"
#include "menu.cpp"


string currencyText;
int currency;
int health = 100, attack = 25, speed = 3, firerate = 1, critrate = 0;
int currentWave = 0;
bool waveActive = false;
TTF_Font* font = nullptr;
Player player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 40, 40, speed, health, health};
GameState gameState = MENU;

void initializeGame() {
    loadGame(currency, health);
    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        exit(1);
    }

    font = TTF_OpenFont("OpenSans-Italic-VariableFont_wdth,wght.ttf", 24);
    if (!font) {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        exit(1);
    }
}

void restartGame() {
    player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 40, 40, 3, health, health };
    enemies.clear();
    bullets.clear();
    currentWave = 0;
    waveActive = false;
    gameState = MENU;
}

void renderCurrency(SDL_Renderer* renderer, int currency){
    SDL_Color textColor = {255,255,0};
    currencyText ="Currency: " + to_string(currency);
    SDL_Surface* surface = TTF_RenderText_Solid(font, currencyText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {SCREEN_WIDTH - surface->w - 10,10, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void renderDeathScreen(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color textColor = {255, 0, 0}; 
    SDL_Surface* surface = TTF_RenderText_Solid(font, "You Died! Press R to return to Menu", textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = { SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT / 2 - surface->h / 2, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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

void close(SDL_Window* window, SDL_Renderer* renderer) {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {

    bool running = true;
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; 

    Uint32 frameStart;
    int frameTime;

    int maxWaves = 10;
    Uint32 lastWaveTime = SDL_GetTicks();
    int enemiesSpawned = 0;
    Uint32 lastEnemySpawnTime = SDL_GetTicks();
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (gameState == MENU || gameState == UPGRADES){
                handleMenuInput(event, running, gameState);
            }
            else if (gameState == GAME || gameState == PAUSED || gameState == DEAD) {
                handleGameInput(event, running, gameState);
            }
        }

        if (gameState == MENU || gameState == UPGRADES) {
            renderMenu(renderer, font, gameState);
        }
        else if (gameState == PAUSED){
            renderPauseMenu(renderer, font);
        }
        else if (gameState == DEAD) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            renderDeathScreen(renderer, font);
            SDL_RenderPresent(renderer);
        }        
        else if (gameState == GAME) {
            frameStart = SDL_GetTicks();
            
            if (player.health<=0){
                gameState = DEAD;
            }
            updatePlayerMovement(player);
            updateShooting(player);
            updateCollisions(player, enemies, bullets);
            spawnWaves(currentWave, maxWaves, waveActive, lastWaveTime, enemiesSpawned, lastEnemySpawnTime);
            for (auto& enemy : enemies) {
                enemy.moveTowardPlayer(player);
            }
            for (auto& bullet : bullets){
                bullet.move();
            }
            bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) {
                return bullet.x < 0 || bullet.x > SCREEN_WIDTH || bullet.y < 0 || bullet.y > SCREEN_HEIGHT;
            }), bullets.end());        

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
            SDL_RenderClear(renderer);
            
            renderPlayer(renderer, player);
            renderEnemies(renderer);
            renderBullets(renderer);
            renderHealth(renderer, player);
            renderRemainHealth(renderer,player);
            renderCurrency(renderer, currency);

            SDL_RenderPresent(renderer);

            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
        }
    }
}
