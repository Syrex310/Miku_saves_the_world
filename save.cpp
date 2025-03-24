#include "common.hpp"
using namespace std;
void saveGame(int currency, int upgradeLevel) {
    ofstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile << currency << endl;
        saveFile << upgradeLevel << endl;
        saveFile.close();
        cout << "Game saved!" << endl;
    } else {
        cout << "Failed to save game!" << endl;
    }
}
void loadGame(int &currency, int &upgradeLevel) {
    ifstream saveFile("save.txt");
    if (saveFile.is_open()) {
        saveFile >> currency;
        saveFile >> upgradeLevel;
        saveFile.close();
        cout << "Game loaded!" << endl;
    } else {
        cout << "No save file found, starting fresh." << endl;
    }
}
