#include "GameState.hpp"
#include "../utils/Logger.hpp"
#include <fstream>
#include <iostream>

GameState::GameState() {
    reset();
}

GameState::~GameState() = default;

void GameState::reset() {
    currentLevel = 1;
    currentLocation = "desktop";
    completed = false;
    score = 1000;
    commandCount = 0;
    discoveries = 0;
    hints = 0;
    flags.clear();
    inventory.clear();
    startTime = std::chrono::steady_clock::now();
}

bool GameState::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            Logger::getInstance().log("Save file not found: " + filename);
            return false;
        }

        nlohmann::json j;
        file >> j;
        fromJson(j);

        Logger::getInstance().log("Game state loaded from: " + filename);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().log("Error loading game state: " + std::string(e.what()));
        return false;
    }
}

bool GameState::saveToFile(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            Logger::getInstance().log("Cannot create save file: " + filename);
            return false;
        }

        nlohmann::json j = toJson();
        file << j.dump(4);

        Logger::getInstance().log("Game state saved to: " + filename);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().log("Error saving game state: " + std::string(e.what()));
        return false;
    }
}

bool GameState::getFlag(const std::string& flagName) const {
    auto it = flags.find(flagName);
    return it != flags.end() ? it->second : false;
}

void GameState::setFlag(const std::string& flagName, bool value) {
    flags[flagName] = value;
}

void GameState::addToInventory(const std::string& item) {
    inventory[item] = true;
}

void GameState::removeFromInventory(const std::string& item) {
    inventory.erase(item);
}

bool GameState::hasInInventory(const std::string& item) const {
    auto it = inventory.find(item);
    return it != inventory.end() && it->second;
}

nlohmann::json GameState::toJson() const {
    nlohmann::json j;

    j["save_info"] = {
        {"level", currentLevel},
        {"location", currentLocation},
        {"save_date", std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count()},
        {"completed", completed}
    };

    j["player_stats"] = {
        {"score", score},
        {"command_count", commandCount},
        {"discoveries", discoveries},
        {"hints", hints}
    };

    j["flags"] = flags;
    j["inventory"] = inventory;

    return j;
}

void GameState::fromJson(const nlohmann::json& j) {
    try {
        if (j.contains("save_info")) {
            auto saveInfo = j["save_info"];
            currentLevel = saveInfo.value("level", 1);
            currentLocation = saveInfo.value("location", "desktop");
            completed = saveInfo.value("completed", false);
        }

        if (j.contains("player_stats")) {
            auto stats = j["player_stats"];
            score = stats.value("score", 1000);
            commandCount = stats.value("command_count", 0);
            discoveries = stats.value("discoveries", 0);
            hints = stats.value("hints", 0);
        }

        if (j.contains("flags")) {
            flags = j["flags"];
        }

        if (j.contains("inventory")) {
            inventory = j["inventory"];
        }

    } catch (const std::exception& e) {
        Logger::getInstance().log("Error parsing JSON: " + std::string(e.what()));
        reset(); // Reset to default state on error
    }
}
