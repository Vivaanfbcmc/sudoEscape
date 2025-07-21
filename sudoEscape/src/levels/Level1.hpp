#pragma once
#include "Level.hpp"
#include "../filesystem/VirtualFileSystem.hpp"
#include "../parser/Commands.hpp"
#include <memory>

class Level1 : public Level {
public:
    Level1(GameState& gameState, ScoreManager& scoreManager);
    ~Level1() override;

    void initialize() override;
    bool executeCommand(const CommandResult& command) override;
    bool isCompleted() const override;
    void reset() override;

    std::string getName() const override { return "The Compromised Server"; }
    std::string getDescription() const override;
    std::string getGoal() const override { return "Find the exit code: EXIT_CODE_CYBER_SECURE_2025"; }

    void showHint() override;
    void showProgress() override;

private:
    VirtualFileSystem vfs;
    std::unique_ptr<Commands> commands;
    std::chrono::steady_clock::time_point startTime;

    void displayInitialMessage();
};
