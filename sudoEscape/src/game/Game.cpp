#include "Game.hpp"
#include "MenuSystem.hpp"
#include "../utils/Logger.hpp"
#include "../test_json_debug.hpp"
#include <iostream>
#include <thread>
#include <chrono>

Game::Game() {
    gameState = std::make_unique<GameState>();
    commandParser = std::make_unique<CommandParser>();
    scoreManager = std::make_unique<ScoreManager>();
}

Game::~Game() = default;

void Game::startNewGame() {
    Logger::getInstance().log("Starting new game");

    // Debug JSON loading at startup
    debugLoadJson("../data/levels/level1.json");

    // Initialize fresh game state
    gameState->reset();
    gameState->setCurrentLevel(1);
    gameState->setCurrentLocation("desktop");
    gameState->setStartTime(std::chrono::steady_clock::now());

    // Initialize Level 1
    currentLevel = std::make_unique<Level1>(*gameState, *scoreManager);

    // Start the game loop
    gameLoop();
}

void Game::resumeGame() {
    Logger::getInstance().log("Resuming game");

    // Initialize the current level based on saved state
    if (gameState->getCurrentLevel() == 1) {
        currentLevel = std::make_unique<Level1>(*gameState, *scoreManager);
    }

    // Resume the game loop
    gameLoop();
}

bool Game::loadGame() {
    try {
        return gameState->loadFromFile("data/gamestate.json");
    } catch (const std::exception& e) {
        Logger::getInstance().log("Failed to load game: " + std::string(e.what()));
        return false;
    }
}

bool Game::saveGame() {
    try {
        return gameState->saveToFile("data/gamestate.json");
    } catch (const std::exception& e) {
        Logger::getInstance().log("Failed to save game: " + std::string(e.what()));
        return false;
    }
}

void Game::gameLoop() {
    std::string command;

    // Display welcome message
    std::cout << "\n=== Welcome to sudoEscape - Level 1: The Compromised Server ===\n";
    std::cout << "You are a digital forensics investigator at DataCorp.\n";
    std::cout << "A security breach occurred overnight. Find the exit code!\n";
    std::cout << "Type 'help' for available commands or 'pause' to access the pause menu.\n\n";

    // Initialize current level
    currentLevel->initialize();

    while (!isGameComplete()) {
        displayGameStatus();

        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "pause") {
            showPauseMenu();
            continue;
        }

        processCommand(command);
    }

    // Game completed
    std::cout << "\n🎉 Congratulations! You've completed Level 1!\n";
    scoreManager->displayFinalScore();
}

void Game::processCommand(const std::string& command) {
    if (command.empty()) return;

    // Parse and execute command
    auto result = commandParser->parseCommand(command);

    if (result.isValid) {
        bool success = currentLevel->executeCommand(result);
        scoreManager->recordCommand(command, success);

        if (success) {
            Logger::getInstance().log("Command executed: " + command);
        } else {
            Logger::getInstance().log("Command failed: " + command);
        }
    } else {
        std::cout << "Invalid command: " << command << "\n";
        std::cout << "Type 'help' for available commands.\n";
        scoreManager->recordCommand(command, false);
    }
}

void Game::showPauseMenu() {
    MenuSystem menuSystem;
    int choice = menuSystem.showPauseMenu();

    switch (choice) {
        case 1: // Resume
            return;
        case 2: // Save & Continue
            saveGame();
            std::cout << "Game saved successfully!\n";
            break;
        case 3: // Save & Quit
            saveGame();
            std::cout << "Game saved. Returning to main menu...\n";
            exit(0);
        case 4: // Restart Level
            startNewGame();
            break;
        default:
            std::cout << "Invalid choice.\n";
    }
}

bool Game::isGameComplete() {
    return gameState->isCompleted();
}

void Game::displayGameStatus() {
    std::cout << "\n--- Current Status ---\n";
    std::cout << "Location: " << gameState->getCurrentLocation() << "\n";
    std::cout << "Commands used: " << scoreManager->getCommandCount() << "\n";
    std::cout << "Current score: " << scoreManager->getCurrentScore() << "\n";
    std::cout << "----------------------\n";
}

void Game::initializeGame() {
    // This method can be used for additional initialization if needed
}
