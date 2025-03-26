#include "common.hpp"
extern void purchaseUpgrade(Player& player);
extern Player player;

void renderPauseMenu(SDL_Renderer* renderer, TTF_Font* font) {
    int buttonWidth = 300;
    int buttonHeight = 60;
    int centerX = (1600 - buttonWidth) / 2;
    int centerY = (900 - buttonHeight) / 2;

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect bg = { centerX - 20, centerY - 40, buttonWidth + 40, buttonHeight * 3 + 80 };
    SDL_RenderFillRect(renderer, &bg);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_Rect resumeButton = { centerX, centerY, buttonWidth, buttonHeight };
    SDL_Rect menuButton = { centerX, centerY + 80, buttonWidth, buttonHeight };
    SDL_Rect exitButton = { centerX, centerY + 160, buttonWidth, buttonHeight };

    SDL_RenderFillRect(renderer, &resumeButton);
    SDL_RenderFillRect(renderer, &menuButton);
    SDL_RenderFillRect(renderer, &exitButton);

    renderText(renderer, font, "Resume", centerX + 90, centerY + 15);
    renderText(renderer, font, "Main Menu", centerX + 75, centerY + 95);
    renderText(renderer, font, "Exit Game", centerX + 80, centerY + 175);

    SDL_RenderPresent(renderer);
}

void handleMenuInput(SDL_Event& event, bool& running, GameState& gameState) {
    if (event.type == SDL_QUIT) {
        saveGame(currency);
        running = false;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        cout << "Mouse Clicked at: " << mouseX << ", " << mouseY << endl;

        int buttonWidth = 200;
        int buttonHeight = 50;
        int centerX = (1600 - buttonWidth) / 2;
        int centerY = (900 - buttonHeight) / 2;
        int startY = 300;
        int spacing = 100;

        if (gameState == MENU) {  
            if (mouseX > centerX && mouseX < centerX + buttonWidth) {  
                if (mouseY > startY && mouseY < startY + buttonHeight) {
                    gameState = GAME;
                    cout << "Start Game clicked!" << endl;
                }
                if (mouseY > startY + spacing && mouseY < startY + spacing + buttonHeight) {
                    gameState = UPGRADES;
                    cout << "Upgrades clicked!" << endl;
                }
                if (mouseY > startY + spacing * 2 && mouseY < startY + spacing * 2 + buttonHeight) {
                    running = false;
                    cout << "Exit clicked!" << endl;
                }
            }
        } 
        else if (gameState == UPGRADES) {  
            if (mouseX > centerX && mouseX < centerX + buttonWidth) {  
                if (mouseY > startY && mouseY < startY + buttonHeight) { 
                    purchaseUpgrade(player);
                    cout << "Upgrade clicked!" << endl;
                }
                if (mouseY > startY + spacing && mouseY < startY + spacing + buttonHeight) { 
                    gameState = MENU;
                    cout << "Back clicked!" << endl;
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
        string currencyDisplay = "Currency: " + to_string(currency);
        renderText(renderer, font, currencyDisplay.c_str(), centerX + 50, startY - 50);


        renderText(renderer, font, "Buy Upgrade (50)", centerX + 20, startY + 15);
        renderText(renderer, font, "Back", centerX + 85, startY + spacing + 15);
    }

    SDL_RenderPresent(renderer);
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