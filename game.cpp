#include "common.hpp"
#include "save.cpp"
#include "enemy.cpp"
#include "player.cpp"
#include "collision.cpp"
#include "bullet.cpp"
#include "menu.cpp"
#include "gif.cpp"
#include "gun.cpp"


string currencyText;
int currency;
int health = 100, attack = 25, speed = 3, firerate = 1, critrate = 0;
int currentWave = 0;
bool waveActive = false;
TTF_Font *font24 = nullptr, *font50 = nullptr;
Player player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 49, 54, speed, health, health};
Enemy enemy;
GameState gameState = MENU;
SDL_Texture* playerTexture = nullptr;
SDL_Texture* enemyTexture = nullptr;
SDL_Texture* menu1 = nullptr, *menu2 = nullptr, *menu3 = nullptr, *bullet2 = nullptr, *ingame = nullptr, *player1 = nullptr, *player2 = nullptr, *gun = nullptr, *backG = nullptr, *pausemenu = nullptr;
SDL_Color black = {0, 0, 0}, white = {255, 255, 255}, blue1 = {109, 198, 254}, blue2 = {106, 202, 242}, blue3 = {85, 151, 190};
bool checkWave = false;
int enemiesLeft = 0;
Mix_Chunk *sShot = nullptr, *sEnemydeath = nullptr, *sClick = nullptr;
Mix_Music *sBackground = nullptr;


SDL_Texture* LoadTexture(const char* file, SDL_Renderer* renderer) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        cout << "Failed to load image: " << IMG_GetError() << endl;
        return nullptr;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void initializeGame(SDL_Renderer* renderer) {
    loadGame(currency, health);

    font24 = TTF_OpenFont("font/VCR_OSD_MONO_1.001.ttf", 24);
    font50 = TTF_OpenFont("font/VCR_OSD_MONO_1.001.ttf", 50);
    playerTexture = LoadTexture("image/player.png", renderer);
    enemyTexture = LoadTexture("image/enemy.png", renderer);
    menu1 = LoadTexture("image/menu1.png", renderer);
    menu2= LoadTexture("image/menu2.png", renderer);
    menu3 = LoadTexture("image/menu3.png", renderer);
    bullet2 = LoadTexture("image/bullet4.png", renderer);
    ingame = LoadTexture("image/ingame.png", renderer);
    player1 = LoadTexture("image/char1.png", renderer);
    player2 = LoadTexture("image/char2.png", renderer);
    gun = LoadTexture("image/gun.png", renderer);
    backG = LoadTexture("image/backG.png", renderer);
    pausemenu = LoadTexture("image/pausemenu.png", renderer);

    sShot = Mix_LoadWAV("sound/shot.wav");
    Mix_VolumeChunk(sShot, 10);
    sEnemydeath = Mix_LoadWAV("sound/cat_meow.wav");
    Mix_VolumeChunk(sEnemydeath, 32);
    sClick = Mix_LoadWAV("sound/click.wav");
    Mix_VolumeChunk(sClick, 96);

    loadGifFrames(renderer);
}


void restartGame() {
    player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 40, 40, 3, health, health };
    enemies.clear();
    bullets.clear();
    currentWave = 0;
    waveActive = false;
    enemy.health = 50;
    enemy.speed = 3;
    player.health = player.maxhealth;
    checkWave = false;
    Mix_VolumeMusic(32);
    switchMusic(sBackground, "sound/Triple5 Here - Crystallogy.mp3");
    gameState = MENU;
}

void renderCurrency(SDL_Renderer* renderer, int currency){
    currencyText ="Currency: " + to_string(currency);
    SDL_Surface* surface = TTF_RenderText_Solid(font50, currencyText.c_str(), black);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect text = {SCREEN_WIDTH - surface->w - 25,25, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &text);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void renderDeathScreen(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color textColor = {255, 0, 0}; 
    SDL_Surface* surface = TTF_RenderText_Solid(font, "YOU DIED! PRESS R TO RETURN TO MENU D:", textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = { SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT / 2 - surface->h / 2, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void renderBackGround(SDL_Renderer *renderer){
    SDL_Rect bg = {0, 0, 1600, 900};
    SDL_RenderCopy(renderer, backG, NULL, &bg);
}
void switchMusic(Mix_Music*& oldMusic, const char* newMusicFile) {
    Mix_HaltMusic();
    if (oldMusic) {
        Mix_FreeMusic(oldMusic);
        oldMusic = nullptr;
    }// stop + free previous msuci

    oldMusic = Mix_LoadMUS(newMusicFile);
    Mix_PlayMusic(oldMusic, -1); // newmusic
}




bool init(SDL_Window*& window, SDL_Renderer*& renderer) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image failed. SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }
    if (TTF_Init() == -1) {
        cout << "SDL_ttf init faield, error: " << TTF_GetError() << endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer faiesd. Error: " << Mix_GetError()<< std::endl;
        return false;
    }
    Mix_VolumeMusic(32);
    Mix_AllocateChannels(128);
    switchMusic(sBackground, "sound/Triple5 Here - Crystallogy.mp3");
    window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window could not be created. SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer failed. SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

void close(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_CloseFont(font24);
    TTF_CloseFont(font50);
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
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
            else if (gameState == GAME || gameState == PAUSED || gameState == DEAD || gameState == WON) {
                handleGameInput(event, running, gameState);
            }
        }

        if (gameState == MENU || gameState == UPGRADES) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            renderGif(renderer);
            renderMenu(renderer, font24, gameState);
            SDL_RenderPresent(renderer);
        }
        else if (gameState == PAUSED){
            renderPauseMenu(renderer, font24);
            SDL_RenderPresent(renderer);
        }
        else if (gameState == DEAD) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            renderDeathScreen(renderer, font50);
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
            spawnWaves(renderer, currentWave, maxWaves, waveActive, lastWaveTime, enemiesSpawned, lastEnemySpawnTime);
            for (auto& enemy : enemies) {
                enemy.moveTowardPlayer(player);
            }
            for (auto& bullet : bullets){
                bullet.move();
            }
            bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) {
                return bullet.x < 0 || bullet.x > SCREEN_WIDTH || bullet.y < 0 || bullet.y > SCREEN_HEIGHT;
            }), bullets.end());        

            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            //renderBackGround(renderer);
            SDL_RenderClear(renderer); // clear old frame

            renderBackGround(renderer);
            renderPlayer(renderer, player);
            renderGun(renderer, gun, player.x + player.width / 2 - 18, player.y + player.height / 2 - 6 + 12);
            renderEnemies(renderer);
            renderBullets(renderer);
            renderCurrency(renderer, currency);
            renderHealth(renderer, player);
            renderRemainHealth(renderer,player);
            renderIngame(renderer);
            if (checkWave == true){
                renderText(renderer, font50, ("Wave " + to_string(currentWave + 1) + " has started. Enemy count: " + to_string((int)(3*pow(1 + currentWave,2)))).c_str(), 800, 200, 800, 200, black);
                enemiesLeft = (int)(3*pow(1 + currentWave,2));
            }
            else{
                int temp1 = enemiesLeft;
                int temp2 = (int)(3*pow(currentWave,2));
                if (currentWave == 5 || currentWave == 10){
                    temp1 +=1;
                    temp2 += 1;
                }
                renderText(renderer, font50, ("Enemies left: " + to_string(temp1) + "/" + to_string(temp2)).c_str(), 800, 800, 800, 800, black);
            }

            SDL_RenderPresent(renderer);

            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
        }
    }
}
