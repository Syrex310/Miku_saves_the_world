#include "common.hpp"
#include "player.cpp"
#include "enemy.cpp"
#include "bullet.cpp"
#include "collision.cpp"
#include "menu.cpp"

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {
    bool running = true;
    SDL_Event event;
    GameState gameState = MENU;

    TTF_Font* font = TTF_OpenFont("OpenSans-Italic-VariableFont_wdth,wght.ttf", 24);


    Player player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50, 3 };
    
    for (auto& bullet : bullets) {
        bullet.move();
    }    

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; 

    Uint32 frameStart;
    int frameTime;

    Uint32 lastSpawnTime = SDL_GetTicks();
    const int spawnInterval = 2000;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (gameState == MENU) {
                handleMenuInput(event, running, gameState);
            }
            else if (gameState == GAME) {
                handleInput(event, player, running);
            }
        }

        if (gameState == MENU) {
            renderMenu(renderer, font);
        }
        else if (gameState == GAME) {
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
    }
}