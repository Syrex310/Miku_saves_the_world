#include "common.hpp"
extern void purchaseUpgrade(Player& player);
extern Player player;

void handleMenuInput(SDL_Event& event, bool& running, GameState& gameState) {
    if (event.type == SDL_QUIT) {
        running = false;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (gameState == MENU) {  
            if (mouseX > 700 && mouseX < 900) {  
                if (mouseY > 300 && mouseY < 350) gameState = GAME;  
                if (mouseY > 400 && mouseY < 450) gameState = UPGRADES;  
                if (mouseY > 500 && mouseY < 550) running = false;  
            }
        } 
        else if (gameState == UPGRADES) {  
            if (mouseX > 700 && mouseX < 900) {  
                if (mouseY > 300 && mouseY < 350) { 
                    purchaseUpgrade(player);
                }
                if (mouseY > 500 && mouseY < 550) { 
                    gameState = MENU;
                }
            }
        }
    }
}

void renderMenu(SDL_Renderer* renderer, TTF_Font* font, GameState gameState) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int buttonWidth = 200;
    int buttonHeight = 50;
    int centerX = (1600 - buttonWidth) / 2;
    int startY = 300;
    int spacing = 100;

    if (gameState == MENU) {
        SDL_Rect startButton = {centerX, startY, buttonWidth, buttonHeight};
        SDL_Rect upgradesButton = {centerX, startY + spacing, buttonWidth, buttonHeight};
        SDL_Rect exitButton = {centerX, startY + spacing * 2, buttonWidth, buttonHeight};

        SDL_RenderFillRect(renderer, &startButton);
        SDL_RenderFillRect(renderer, &upgradesButton);
        SDL_RenderFillRect(renderer, &exitButton);

        renderText(renderer, font, "Start Game", centerX + 50, startY + 15);
        renderText(renderer, font, "Upgrades", centerX + 60, startY + spacing + 15);
        renderText(renderer, font, "Exit", centerX + 85, startY + spacing * 2 + 15);
    } 
    else if (gameState == UPGRADES) {
        SDL_Rect upgradeButton = {centerX, startY, buttonWidth, buttonHeight};
        SDL_Rect backButton = {centerX, startY + spacing, buttonWidth, buttonHeight};

        SDL_RenderFillRect(renderer, &upgradeButton);
        SDL_RenderFillRect(renderer, &backButton);

        renderText(renderer, font, "Buy Upgrade (50)", centerX + 20, startY + 15);
        renderText(renderer, font, "Back", centerX + 85, startY + spacing + 15);
    }

    SDL_RenderPresent(renderer);
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