
#include "common.hpp"
using namespace std;

void saveGame(int currency) {
    ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << currency << endl;
        //saveFile << upgradeLevel << endl;
        saveFile.close();
        cout << "Game saved!" << endl;
    } else {
        cout << "Failed to save game!" << endl;
    }
}
void loadGame(int &currency) {
    ifstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile >> currency;
        //saveFile >> upgradeLevel;
        saveFile.close();
        cout << "Game loaded!" << endl;
    } else {
        cout << "No save file found, starting fresh." << endl;
    }
}
void purchaseUpgrade(Player& player) {
    int upgradeCost = 50;

    if (currency >= upgradeCost) {
        player.health += 10;
        currency -= upgradeCost;
        saveGame(currency);

        cout << "Upgrade purchased! Health increased by 10, remaining currency: " << currency << endl;
    } else {
        cout << "Not enough currency for upgrade!" << endl;
    }
}
