#pragma once
#include <string>
#include <unordered_map>
#include <chrono>
#include <nlohmann/json.hpp>

class GameState {
public:
    GameState();
    ~GameState();

    // Game state management
    void reset();
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);

    // Level management
    int getCurrentLevel() const { return currentLevel; }
    void setCurrentLevel(int level) { currentLevel = level; }

    // Location management
    std::string getCurrentLocation() const { return currentLocation; }
    void setCurrentLocation(const std::string& location) { currentLocation = location; }

    // Progress tracking
    bool isCompleted() const { return completed; }
    void setCompleted(bool state) { completed = state; }

    // Flag management
    bool getFlag(const std::string& flagName) const;
    void setFlag(const std::string& flagName, bool value);

    // Time tracking
    void setStartTime(std::chrono::steady_clock::time_point time) { startTime = time; }
    std::chrono::steady_clock::time_point getStartTime() const { return startTime; }

    // Score and stats
    int getScore() const { return score; }
    void setScore(int newScore) { score = newScore; }

    int getCommandCount() const { return commandCount; }
    void setCommandCount(int count) { commandCount = count; }

    // Inventory management
    void addToInventory(const std::string& item);
    void removeFromInventory(const std::string& item);
    bool hasInInventory(const std::string& item) const;

    // Player stats
    void incrementDiscoveries() { discoveries++; }
    int getDiscoveries() const { return discoveries; }

    void incrementHints() { hints++; }
    int getHints() const { return hints; }

private:
    int currentLevel;
    std::string currentLocation;
    bool completed;
    int score;
    int commandCount;
    int discoveries;
    int hints;

    std::unordered_map<std::string, bool> flags;
    std::unordered_map<std::string, bool> inventory;
    std::chrono::steady_clock::time_point startTime;

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
};
