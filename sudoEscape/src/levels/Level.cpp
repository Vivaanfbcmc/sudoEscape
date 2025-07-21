#include "Level.hpp"
#include "../utils/Logger.hpp"
#include <iostream>
#include <iomanip>

Level::Level(GameState& gs, ScoreManager& sm) 
    : gameState(gs), scoreManager(sm) {
    updateLastActivity();
}

Level::~Level() = default;

void Level::displayMessage(const std::string& message) const {
    std::cout << "\n" << message << "\n";
}

void Level::displayError(const std::string& error) const {
    std::cout << "\n❌ " << error << "\n";
}

void Level::displaySuccess(const std::string& success) const {
    std::cout << "\n✅ " << success << "\n";
}

void Level::displayDiscovery(const std::string& discovery) const {
    std::cout << "\n🔍 DISCOVERY: " << discovery << "\n";
    scoreManager.recordDiscovery();
}

void Level::markStageComplete(const std::string& stageName) {
    std::string flagName = "stage_" + stageName + "_complete";
    gameState.setFlag(flagName, true);
    Logger::getInstance().log("Stage completed: " + stageName);
}

bool Level::isStageComplete(const std::string& stageName) const {
    std::string flagName = "stage_" + stageName + "_complete";
    return gameState.getFlag(flagName);
}

void Level::updateLastActivity() {
    lastActivity = std::chrono::steady_clock::now();
}

std::chrono::steady_clock::time_point Level::getLastActivity() const {
    return lastActivity;
}
