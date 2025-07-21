#pragma once
#include <memory>
#include <string>
#include "GameState.hpp"
#include "../parser/CommandParser.hpp"
#include "../levels/Level1.hpp"
#include "../scoring/ScoreManager.hpp"

class Game {
public:
    Game();
    ~Game();

    void startNewGame();
    void resumeGame();
    bool loadGame();
    bool saveGame();
    void gameLoop();

    GameState& getGameState() { return *gameState; }

private:
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<CommandParser> commandParser;
    std::unique_ptr<Level1> currentLevel;
    std::unique_ptr<ScoreManager> scoreManager;

    void initializeGame();
    void processCommand(const std::string& command);
    void showPauseMenu();
    bool isGameComplete();
    void displayGameStatus();
};
