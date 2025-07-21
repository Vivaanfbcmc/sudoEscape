#pragma once
#include <string>

class MenuSystem {
public:
    MenuSystem();
    ~MenuSystem();

    int showMainMenu();
    int showPauseMenu();
    void displayLogo();
    void clearScreen();

private:
    void displayMainMenuOptions();
    void displayPauseMenuOptions();
    int getIntInput(int min, int max);
    std::string readLogoFromFile();
};
