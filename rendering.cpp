#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern std::vector<Enemy> enemies;
extern std::vector<Bullet> bullets;

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

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y) {
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        printf("Text rendering failed! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void renderMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int buttonWidth = 200;
    int buttonHeight = 50;
    int centerX = (1600 - buttonWidth) / 2;
    int startY = 300;
    int spacing = 100;

    SDL_Rect startButton = {centerX, startY, buttonWidth, buttonHeight};
    SDL_Rect upgradesButton = {centerX, startY + spacing, buttonWidth, buttonHeight};
    SDL_Rect exitButton = {centerX, startY + spacing * 2, buttonWidth, buttonHeight};

    SDL_RenderFillRect(renderer, &startButton);
    SDL_RenderFillRect(renderer, &upgradesButton);
    SDL_RenderFillRect(renderer, &exitButton);

    renderText(renderer, font, "Start Game", centerX + 50, startY + 15);
    renderText(renderer, font, "Upgrades", centerX + 60, startY + spacing + 15);
    renderText(renderer, font, "Exit", centerX + 85, startY + spacing * 2 + 15);

    SDL_RenderPresent(renderer);
}
