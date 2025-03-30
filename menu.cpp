#include "common.hpp"


void renderPauseMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Rect pausem = {0, 0, 1600, 900};
    SDL_RenderCopy(renderer, pausemenu, NULL, &pausem);
}


void handleMenuInput(SDL_Event& event, bool& running, GameState& gameState) {
    if (event.type == SDL_QUIT) {
        saveGame();
        running = false;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        Mix_PlayChannel(-1, sClick, 0);
        //cout << "Mouse Clicked at: " << mouseX << ", " << mouseY << endl;

        if (gameState == MENU){
            if (mouseX < 700 && mouseY > 100 && mouseY < 300) {
                Mix_VolumeMusic(24);
                switchMusic(sBackground, "AudioCoffee Band - Digital Technology.mp3");
                gameState = GAME;
                cout<<"Start/Continue Game"<<endl;
            }
            else if (mouseX < 600 && mouseY > 325 && mouseY < 525) {
                switchMusic(sBackground, "AudioCoffee Band - Pure Inspiration.mp3");
                gameState = UPGRADES;
                cout<<"Upgrade Menu Clicked"<<endl;
            }
            else if (mouseX < 500 && mouseY > 550 && mouseY < 750) {
                running = false; saveGame();
                cout<<"Exit Game"<<endl;
            }
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
                switchMusic(sBackground, "Triple5 Here - Crystallogy.mp3");
                gameState = MENU;
                cout<<"Returned to MENU"<<endl;
            }
        }
        else if (gameState == WON){
            Mix_VolumeMusic(32);
            switchMusic(sBackground, "Triple5 Here - Crystallogy.mp3");
            gameState = MENU;
        }
    }
}


void renderMenu(SDL_Renderer* renderer, TTF_Font* font, GameState gameState) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

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
        //SDL_Rect rect2 = {0, 0, 1600, 900};
        //SDL_RenderCopy(renderer, menu3, NULL, &rect2);
        SDL_Rect rect = {0, 0, 1600, 900};
        SDL_RenderCopy(renderer, menu2, NULL, &rect);

        renderText(renderer, font50, ("COINS: " + to_string(currency)).c_str(), 1050, 55, 1550, 105, white);

        renderText(renderer, font50, to_string(HP_upCost * HP_stage).c_str(), 50, 400, 210, 525, blue1);
        renderText(renderer, font50, to_string(ATK_upCost * ATK_stage).c_str(), 235, 400, 395, 525, blue1);
        renderText(renderer, font50, to_string(SPEED_upCost * SPEED_stage).c_str(), 420, 400, 580, 525, blue1);
        renderText(renderer, font50, to_string(FR_upCost * FR_stage).c_str(), 605, 400, 765, 525, blue1);
        renderText(renderer, font50, to_string(CR_upCost * CR_stage).c_str(), 790, 400, 950, 525, blue1);

        renderText(renderer, font50, (to_string(player.maxhealth)).c_str(), 235, 650, 550, 650, blue1);
        renderText(renderer, font50, to_string(attack).c_str(), 790, 650, 940, 650, blue1);
        renderText(renderer, font50, to_string(player.speed).c_str(), 235, 725, 550, 725, blue1);
        renderText(renderer, font50, to_string(firerate).c_str(), 790, 725, 940, 725, blue1);
        renderText(renderer, font50, to_string(critrate).c_str(), 235, 800, 550, 800, blue1);
        renderText(renderer, font50, "2", 790, 800, 940, 800, blue1);
    }

}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x1, int y1, int x2, int y2, SDL_Color textColor) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        cout << "SDL_TTF FAILED: " << TTF_GetError() << endl;
        return;
    }
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    int centerX = x1 + (x2 - x1) / 2 - textWidth / 2;
    int centerY = y1 + (y2 - y1) / 2 - textHeight / 2;

    SDL_Rect textRect = {centerX, centerY, textWidth, textHeight};

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}
