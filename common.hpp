#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <string>


using namespace std;

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
enum GameState { MENU, GAME, UPGRADES, PAUSED, DEAD, WON};
enum UpgradeType { HEALTH, ATTACK, SPEED, FIRERATE, CRITRATE};

struct Player {
    int x, y;
    int width, height;
    int speed;
    int health;
    int maxhealth;
    Uint32 lastDamageTime = 0;
};

struct Enemy {
    int x, y;
    int width, height;
    int speed;
    int health;
    void moveTowardPlayer(Player& player);
};


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
    void move();
};



bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void updateCollisions(Player& player, vector<Enemy>& enemies, vector<Bullet>& bullets);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x1, int y1, int x2, int y2, SDL_Color textColor);
void saveGame();
void loadGame(int &currency, int &health);
void Upgrade(Player& player, UpgradeType type);
void renderGif(SDL_Renderer* renderer);
void restartGame();


extern Player player;
extern Enemy enemy;
extern int currency;
extern int health, attack, speed, firerate, critrate;
extern int HP_stage, ATK_stage, SPEED_stage, FR_stage, CR_stage;
extern int currentWave;
extern SDL_Texture* LoadTexture(const char* file, SDL_Renderer* renderer);
extern vector<Bullet> bullets;
extern vector<Enemy> enemies;
extern GameState gameState;
extern SDL_Texture *playerTexture, *enemyTexture, *menu1, *menu2, *menu3, *bullet2, *ingame, *player1, *player2, *gun, *backG;
extern TTF_Font *font24, *font50;
extern SDL_Color white, black, blue1;
extern bool checkWave;
extern int enemiesLeft;