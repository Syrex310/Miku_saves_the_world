#include "common.hpp"


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
        saveGame(currency, health);
        running = false;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        //cout << "Mouse Clicked at: " << mouseX << ", " << mouseY << endl;

        if (gameState == MENU){
            if (mouseX < 700 && mouseY > 100 && mouseY < 300) {gameState = GAME; cout<<"Start/Continue Game"<<endl;}
            else if (mouseX < 600 && mouseY > 325 && mouseY < 525) {gameState = UPGRADES; cout<<"Upgrade Menu Clicked"<<endl;}
            else if (mouseX < 500 && mouseY > 550 && mouseY < 750) {running = false; cout<<"Exit Game"<<endl;}
        }
        else if (gameState == UPGRADES) {  
            if (mouseX > 50 && mouseX < 950){
                if (mouseY > 50 && mouseY < 550){
                    if (mouseX < 210) Upgrade(player, HEALTH);
                    else if (mouseX > 235 && mouseX < 395) Upgrade(player, ATTACK);
                    else if (mouseX > 420 && mouseX < 580) Upgrade(player, SPEED);
                    else if (mouseX > 605 && mouseX < 765) Upgrade(player, FIRERATE);
                    else if (mouseX > 790 && mouseX < 950) Upgrade(player, CRITRATE);
                }
            }
            else if (mouseX > 1050 && mouseX < 1550 && mouseY > 600 && mouseY < 850){
                gameState = MENU;
                cout<<"Returned to MENU"<<endl;
            }
        }
        else if (gameState == WON){
            gameState = MENU;
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
        SDL_Rect rect = {0, 0, 1600, 900};
        SDL_RenderCopy(renderer, menu1, NULL, &rect);
    } 
    else if (gameState == UPGRADES) {
        SDL_Rect rect = {0, 0, 1600, 900};
        SDL_RenderCopy(renderer, menu2, NULL, &rect);
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