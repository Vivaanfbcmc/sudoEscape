#pragma once
#include <string>
#include <memory>
#include "../game/GameState.hpp"
#include "../parser/CommandParser.hpp"
#include "../scoring/ScoreManager.hpp"

class Level {
public:
    Level(GameState& gameState, ScoreManager& scoreManager);
    virtual ~Level();

    // Level lifecycle
    virtual void initialize() = 0;
    virtual bool executeCommand(const CommandResult& command) = 0;
    virtual bool isCompleted() const = 0;
    virtual void reset() = 0;

    // Information
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getGoal() const = 0;

    // Hints and guidance
    virtual void showHint() = 0;
    virtual void showProgress() = 0;

protected:
    GameState& gameState;
    ScoreManager& scoreManager;

    // Helper methods
    void displayMessage(const std::string& message) const;
    void displayError(const std::string& error) const;
    void displaySuccess(const std::string& success) const;
    void displayDiscovery(const std::string& discovery) const;

    // Progress tracking
    void markStageComplete(const std::string& stageName);
    bool isStageComplete(const std::string& stageName) const;

    // Time tracking
    void updateLastActivity();
    std::chrono::steady_clock::time_point getLastActivity() const;

private:
    std::chrono::steady_clock::time_point lastActivity;
};
