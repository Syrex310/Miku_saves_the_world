#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cmath>

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
enum GameState { MENU, GAME, UPGRADES };

struct Player {
    int x, y;
    int width, height;
    int speed;
    int health = 10;
    Uint32 lastDamageTime = 0;
};

struct Enemy {
    int x, y;
    int width, height;
    int speed;
    int health = 50;
    void moveTowardPlayer(Player& player);
};

struct Bullet {
    float x, y;
    float speedX, speedY;
    void move();
};

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void updateCollisions(Player& player, std::vector<Enemy>& enemies, std::vector<Bullet>& bullets);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);
