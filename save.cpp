
#include "common.hpp"

#include <iostream>
#include <fstream>

using namespace std;
int HP_upCost = 50, ATK_upCost = 100, SPEED_upCost = 50, FR_upCost = 100, CR_upCost = 500; 
int HP_stage = 1, ATK_stage = 1, SPEED_stage = 1, FR_stage = 1, CR_stage = 1;

void saveGame(int currency, int health) {
    ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << currency << '\n';
        saveFile << health << '\n';
        saveFile << attack << '\n';
        saveFile << speed << '\n';
        saveFile << firerate << '\n';
        saveFile << critrate << '\n';
        saveFile << HP_stage << '\n';
        saveFile << ATK_stage << '\n';
        saveFile << SPEED_stage << '\n';
        saveFile << FR_stage << '\n';
        saveFile << CR_stage << '\n';
        saveFile.close();
        cout << "Game saved!" << endl;
    } else {
        cout << "Failed to save game!" << endl;
    }
}

void loadGame(int& currency, int& health) { 
    ifstream saveFile("save.txt");
    if (saveFile.is_open()) {
        if (!(saveFile >> currency >> health >> attack >> speed >> firerate >> critrate >> HP_stage >> ATK_stage >> SPEED_stage >> FR_stage >> CR_stage)) { 
            cout << "Save file corrupted, resetting values." << endl;
            currency = 0;
            health = 100; attack = 25; speed = 3; firerate = 1; critrate = 0;
            HP_stage = 1; ATK_stage = 1; SPEED_stage = 1; FR_stage = 1; CR_stage = 1;
        } else {
            saveFile >> currency >> health >> attack >> speed >> firerate >> critrate >> HP_stage >> ATK_stage >> SPEED_stage >> FR_stage >> CR_stage;
            player.health=health;
            player.maxhealth=health;
            cout << "Game loaded!" << endl;
        }
        saveFile.close();
    } else {
        cout << "No save file found, starting fresh." << endl;
        currency = 0;
        health = 100; attack = 25; speed = 3; firerate = 1; critrate = 0;
        HP_stage = 1; ATK_stage = 1; SPEED_stage = 1; FR_stage = 1; CR_stage = 1;
    }
}

void Upgrade(Player& player, UpgradeType type){
    switch (type){
        case HEALTH:
            if (currency >= HP_upCost * HP_stage) {
                health += 10 * HP_stage;
                currency -= HP_upCost * HP_stage;
                saveGame(currency, health);

                cout << "Upgrade purchased! Health increased by 10, remaining currency: " << currency << endl;
            } else {
                cout << "Not enough currency for upgrade!" << endl;
            }
            break;
        case ATTACK:
            if (currency >= ATK_upCost * ATK_stage) {
                attack += 15;
                currency -= ATK_upCost * ATK_stage;
                saveGame(currency, health);

                cout << "Upgrade purchased! Attack increased by 15, remaining currency: " << currency << endl;
            } else {
                cout << "Not enough currency for upgrade!" << endl;
            }
            break;
        case SPEED:
            if (currency >= SPEED_upCost * SPEED_stage) {
                speed += 0.3;
                currency -= SPEED_upCost * SPEED_stage;
                saveGame(currency, health);

                cout << "Upgrade purchased! Speed increased by 0.3, remaining currency: " << currency << endl;
            } else {
                cout << "Not enough currency for upgrade!" << endl;
            }
            break;
        case FIRERATE:
            if (currency >= FR_upCost * FR_stage) {
                firerate += 0.2;
                currency -= FR_upCost * FR_stage;
                saveGame(currency, health);

                cout << "Upgrade purchased! Fire rate increased by 0.2, remaining currency: " << currency << endl;
            } else {
                cout << "Not enough currency for upgrade!" << endl;
            }
            break;
        case CRITRATE:
            if (currency >= CR_upCost * CR_stage) {
                critrate += 10;
                currency -= CR_upCost * CR_stage;
                saveGame(currency, health);

                cout << "Upgrade purchased! Crit rate increased by 10, remaining currency: " << currency << endl;
            } else {
                cout << "Not enough currency for upgrade!" << endl;
            }
            break;
            
    }
    saveGame(currency, health);
}
