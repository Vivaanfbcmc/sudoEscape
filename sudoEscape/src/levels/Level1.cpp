#include "Level1.hpp"
#include "../utils/Logger.hpp"
#include <iostream>
#include <fstream>

Level1::Level1(GameState& gs, ScoreManager& sm)
    : Level(gs, sm) {
    commands = std::make_unique<Commands>(gameState, vfs);
}

Level1::~Level1() = default;

void Level1::initialize() {
    // Load level JSON
    // Use absolute path to ensure the file is found
    std::string levelPath = "D:/sudoEscape/sudoEscape/data/levels/level1.json";
    std::cout << "[DEBUG] Attempting to load level file: " << levelPath << std::endl;
    Logger::getInstance().log("[DEBUG] Attempting to load level file: " + levelPath);
    if (!vfs.loadFromJson(levelPath)) {
        std::cout << "[ERROR] Failed to load level1.json at: " << levelPath << std::endl;
        Logger::getInstance().log("[ERROR] Failed to load level1.json at: " + levelPath);
    }

    displayInitialMessage();
    startTime = std::chrono::steady_clock::now();
}

bool Level1::executeCommand(const CommandResult& result) {
    // Route to appropriate Commands method
    if (!result.isValid) {
        std::cout << "Invalid command. Type 'help' for assistance." << std::endl;
        return false;
    }

    // Map commands to functions
    const std::string& cmd = result.command;
    const std::vector<std::string>& args = result.args;
    bool success = false;

    if (cmd == "help") success = commands->help();
    else if (cmd == "clear") success = commands->clear();
    else if (cmd == "ls") success = commands->ls(args);
    else if (cmd == "pwd") success = commands->pwd();
    else if (cmd == "cd") success = commands->cd(result.primaryArg);
    else if (cmd == "back") success = commands->back();
    else if (cmd == "open") success = commands->open(result.primaryArg);
    else if (cmd == "cat") success = commands->cat(result.primaryArg);
    else if (cmd == "head") success = commands->head(result.primaryArg);
    else if (cmd == "tail") success = commands->tail(result.primaryArg);
    else if (cmd == "grep" && args.size() >= 2) success = commands->grep(args[0], args[1]);
    else if (cmd == "strings") success = commands->strings(result.primaryArg);
    else if (cmd == "xxd") success = commands->xxd(result.primaryArg);
    else if (cmd == "base64") success = commands->base64(result.primaryArg);
    else if (cmd == "rot13") success = commands->rot13(result.primaryArg);
    else if (cmd == "decode") success = commands->decode(result.primaryArg);
    else if (cmd == "touch") success = commands->touch(result.primaryArg);
    else if (cmd == "create" && args.size() >= 2) {
        std::string content = result.fullCommand.substr(result.fullCommand.find(result.args[1]));
        success = commands->create(result.primaryArg, content);
    }
    else if (cmd == "edit") success = commands->edit(result.primaryArg);
    else if (cmd == "rm" || cmd == "delete") success = commands->deleteFile(result.primaryArg);
    else if (cmd == "count") success = commands->count();
    else if (cmd == "history") success = commands->history();
    else if (cmd == "examine") success = commands->examine(result.primaryArg);
    else if (cmd == "find") success = commands->find(result.primaryArg);
    else if (cmd == "solve") success = commands->solve(result.primaryArg);
    else {
        std::cout << "Command not implemented." << std::endl;
        success = false;
    }

    if (isCompleted()) {
        scoreManager.setEndTime(std::chrono::steady_clock::now());
    }

    return success;
}

bool Level1::isCompleted() const {
    return gameState.isCompleted();
}

void Level1::reset() {
    gameState.reset();
    scoreManager.reset();
    initialize();
}

std::string Level1::getDescription() const {
    return "Investigate a compromised server and find the exit code.";
}

void Level1::showHint() {
    std::cout << "Hint: Check the logs directory for suspicious activity." << std::endl;
    gameState.incrementHints();
}

void Level1::showProgress() {
    std::cout << "Progress: " << (isStageComplete("final") ? "Complete" : "In Progress") << std::endl;
}

void Level1::displayInitialMessage() {
    std::cout << "You arrive at your workstation desktop. Folders and shortcuts await." << std::endl;
}
