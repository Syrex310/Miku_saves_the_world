
#include "common.hpp"

#include <iostream>
#include <fstream>

using namespace std;

void saveGame(int currency, int health) {
    ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << currency << '\n';
        saveFile << health << '\n';
        saveFile.close();
        cout << "Game saved!" << endl;
    } else {
        cout << "Failed to save game!" << endl;
    }
}

void loadGame(int& currency, int& health) { 
    ifstream saveFile("save.txt");
    if (saveFile.is_open()) {
        if (!(saveFile >> currency >> health)) { 
            cout << "Save file corrupted, resetting values." << endl;
            currency = 0;
            health = 100;
        } else {
            saveFile>>currency>>health;
            player.health=health;
            player.maxhealth=health;
            cout << "Game loaded!" << endl;
        }
        saveFile.close();
    } else {
        cout << "No save file found, starting fresh." << endl;
        currency = 0;
        health = 100;
    }
}

void purchaseUpgrade(Player& player) {
    int upgradeCost = 50;

    if (currency >= upgradeCost) {
        health += 10;
        currency -= upgradeCost;
        saveGame(currency, health);

        cout << "Upgrade purchased! Health increased by 10, remaining currency: " << currency << endl;
    } else {
        cout << "Not enough currency for upgrade!" << endl;
    }
}
