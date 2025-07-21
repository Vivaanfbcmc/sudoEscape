#include "MenuSystem.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

MenuSystem::MenuSystem() = default;
MenuSystem::~MenuSystem() = default;

int MenuSystem::showMainMenu() {
    clearScreen();
    displayLogo();
    displayMainMenuOptions();

    return getIntInput(1, 3);
}

int MenuSystem::showPauseMenu() {
    std::cout << "\n=== PAUSE MENU ===\n";
    displayPauseMenuOptions();

    return getIntInput(1, 4);
}

void MenuSystem::displayLogo() {
    std::cout << readLogoFromFile() << std::endl;
}


void MenuSystem::displayMainMenuOptions() {
    std::cout << "\n=== MAIN MENU ===\n";
    std::cout << "1. Start New Game\n";
    std::cout << "2. Continue Game\n";
    std::cout << "3. Exit\n";
    std::cout << "\nEnter your choice (1-3): ";
}

void MenuSystem::displayPauseMenuOptions() {
    std::cout << "1. Resume Game\n";
    std::cout << "2. Save & Continue\n";
    std::cout << "3. Save & Quit\n";
    std::cout << "4. Restart Level\n";
    std::cout << "\nEnter your choice (1-4): ";
}

int MenuSystem::getIntInput(int min, int max) {
    int choice;
    while (true) {
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        } else if (choice < min || choice > max) {
            std::cout << "Invalid choice. Please enter a number between " << min << " and " << max << ": ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

void MenuSystem::clearScreen() {
    // Cross-platform clear screen
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string MenuSystem::readLogoFromFile() {
    std::ifstream file("assets/ascii_art/logo.txt");
    if (!file.is_open()) {
        return "sudoEscape - Terminal Escape Room Game";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
