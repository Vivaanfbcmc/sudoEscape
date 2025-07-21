#include "ScoreManager.hpp"
#include "../utils/Logger.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>


// Static member definitions
const double ScoreManager::PLATINUM_MULTIPLIER = 1.5;
const double ScoreManager::GOLD_MULTIPLIER = 1.3;
const double ScoreManager::SILVER_MULTIPLIER = 1.1;
const double ScoreManager::BRONZE_MULTIPLIER = 1.0;

ScoreManager::ScoreManager() {
    commandCounter = std::make_unique<CommandCounter>();
    reset();
}

ScoreManager::~ScoreManager() = default;

void ScoreManager::recordCommand(const std::string& command, bool success) {
    commandCounter->recordCommand(command, success);
}

int ScoreManager::getCommandCount() const {
    return commandCounter->getTotalCommands();
}

int ScoreManager::getCurrentScore() const {
    return calculateFinalScore();
}

int ScoreManager::getCommandPenalty() const {
    return getCommandCount() * COMMAND_PENALTY;
}

int ScoreManager::getDiscoveryBonus() const {
    return discoveryCount * DISCOVERY_BONUS;
}

int ScoreManager::getHintPenalty() const {
    return hintCount * HINT_PENALTY;
}

int ScoreManager::getTimeBonus() const {
    return calculateTimeBonus();
}

EfficiencyTier ScoreManager::getEfficiencyTier() const {
    int commands = getCommandCount();

    if (commands <= PLATINUM_THRESHOLD) {
        return EfficiencyTier::PLATINUM;
    } else if (commands <= GOLD_THRESHOLD) {
        return EfficiencyTier::GOLD;
    } else if (commands <= SILVER_THRESHOLD) {
        return EfficiencyTier::SILVER;
    } else {
        return EfficiencyTier::BRONZE;
    }
}

double ScoreManager::getEfficiencyMultiplier() const {
    switch (getEfficiencyTier()) {
        case EfficiencyTier::PLATINUM: return PLATINUM_MULTIPLIER;
        case EfficiencyTier::GOLD: return GOLD_MULTIPLIER;
        case EfficiencyTier::SILVER: return SILVER_MULTIPLIER;
        case EfficiencyTier::BRONZE: return BRONZE_MULTIPLIER;
        default: return BRONZE_MULTIPLIER;
    }
}

std::string ScoreManager::getEfficiencyTierName() const {
    switch (getEfficiencyTier()) {
        case EfficiencyTier::PLATINUM: return "Platinum";
        case EfficiencyTier::GOLD: return "Gold";
        case EfficiencyTier::SILVER: return "Silver";
        case EfficiencyTier::BRONZE: return "Bronze";
        default: return "Bronze";
    }
}

void ScoreManager::recordDiscovery() {
    discoveryCount++;
    Logger::getInstance().log("Discovery recorded. Total: " + std::to_string(discoveryCount));
}

void ScoreManager::recordHint() {
    hintCount++;
    Logger::getInstance().log("Hint used. Total: " + std::to_string(hintCount));
}

void ScoreManager::setStartTime(std::chrono::steady_clock::time_point time) {
    startTime = time;
}

void ScoreManager::setEndTime(std::chrono::steady_clock::time_point time) {
    endTime = time;
    gameCompleted = true;
}

std::chrono::milliseconds ScoreManager::getElapsedTime() const {
    auto currentTime = gameCompleted ? endTime : std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
}

double ScoreManager::getSuccessRate() const {
    return commandCounter->getSuccessRate();
}

void ScoreManager::displayCurrentScore() const {
    std::cout << "\n=== CURRENT SCORE ===\n";
    std::cout << "Base Score: " << BASE_SCORE << "\n";
    std::cout << "Command Penalty: -" << getCommandPenalty() << " (" << getCommandCount() << " commands)\n";
    std::cout << "Discovery Bonus: +" << getDiscoveryBonus() << " (" << discoveryCount << " discoveries)\n";
    std::cout << "Hint Penalty: -" << getHintPenalty() << " (" << hintCount << " hints)\n";
    std::cout << "Time Bonus: +" << getTimeBonus() << "\n";
    std::cout << "Efficiency Tier: " << getEfficiencyTierName() << " (" << getEfficiencyMultiplier() << "x)\n";
    std::cout << "Current Score: " << getCurrentScore() << "\n";
    std::cout << "====================\n";
}

void ScoreManager::displayFinalScore() const {
    std::cout << "\n🎯 FINAL SCORE BREAKDOWN 🎯\n";
    std::cout << "==============================\n";
    std::cout << "Base Score:        " << std::setw(6) << BASE_SCORE << "\n";
    std::cout << "Command Penalty:   " << std::setw(6) << -getCommandPenalty() << " (" << getCommandCount() << " commands)\n";
    std::cout << "Discovery Bonus:   " << std::setw(6) << getDiscoveryBonus() << " (" << discoveryCount << " discoveries)\n";
    std::cout << "Hint Penalty:      " << std::setw(6) << -getHintPenalty() << " (" << hintCount << " hints)\n";
    std::cout << "Time Bonus:        " << std::setw(6) << getTimeBonus() << "\n";
    std::cout << "------------------------------\n";
    std::cout << "Subtotal:          " << std::setw(6) << (BASE_SCORE - getCommandPenalty() + getDiscoveryBonus() - getHintPenalty() + getTimeBonus()) << "\n";
    std::cout << "Efficiency Tier:   " << getEfficiencyTierName() << " (" << getEfficiencyMultiplier() << "x)\n";
    std::cout << "==============================\n";
    std::cout << "FINAL SCORE:       " << std::setw(6) << getCurrentScore() << "\n";
    std::cout << "==============================\n";
    std::cout << "Completion Time:   " << formatTime(getElapsedTime()) << "\n";
    std::cout << "Success Rate:      " << std::fixed << std::setprecision(1) << getSuccessRate() << "%\n";
    std::cout << "==============================\n";
}

void ScoreManager::displayDetailedStats() const {
    std::cout << "\n=== DETAILED STATISTICS ===\n";

    // Command statistics
    std::cout << "Commands:\n";
    std::cout << "  Total: " << commandCounter->getTotalCommands() << "\n";
    std::cout << "  Successful: " << commandCounter->getSuccessfulCommands() << "\n";
    std::cout << "  Failed: " << commandCounter->getFailedCommands() << "\n";
    std::cout << "  Invalid: " << commandCounter->getInvalidCommands() << "\n";
    std::cout << "  Success Rate: " << std::fixed << std::setprecision(1) << getSuccessRate() << "%\n";

    // Most used command
    std::string mostUsed = commandCounter->getMostUsedCommand();
    if (!mostUsed.empty()) {
        std::cout << "  Most Used: " << mostUsed << "\n";
    }

    // Time statistics
    std::cout << "\nTime:\n";
    std::cout << "  Total Time: " << formatTime(getElapsedTime()) << "\n";
    std::cout << "  Avg per Command: " << formatTime(commandCounter->getAverageCommandTime()) << "\n";

    // Game events
    std::cout << "\nGame Events:\n";
    std::cout << "  Discoveries: " << discoveryCount << "\n";
    std::cout << "  Hints Used: " << hintCount << "\n";

    std::cout << "===========================\n";
}

void ScoreManager::reset() {
    commandCounter->reset();
    discoveryCount = 0;
    hintCount = 0;
    startTime = std::chrono::steady_clock::now();
    endTime = startTime;
    gameCompleted = false;
}

int ScoreManager::calculateTimeBonus() const {
    if (!gameCompleted) {
        return 0;
    }

    auto elapsed = getElapsedTime();
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(elapsed).count();

    // Maximum bonus for completing in 10 minutes or less
    // Linear decrease to 0 bonus at 30 minutes
    if (minutes <= 10) {
        return MAX_TIME_BONUS;
    } else if (minutes <= 30) {
        return MAX_TIME_BONUS - static_cast<int>((minutes - 10) * (MAX_TIME_BONUS / 20.0));
    } else {
        return 0;
    }
}

int ScoreManager::calculateFinalScore() const {
    int baseScore = BASE_SCORE;
    int penalties = getCommandPenalty() + getHintPenalty();
    int bonuses = getDiscoveryBonus() + getTimeBonus();

    int subtotal = baseScore - penalties + bonuses;

    // Apply efficiency multiplier
    double multiplier = getEfficiencyMultiplier();
    int finalScore = static_cast<int>(subtotal * multiplier);

    // Ensure minimum score of 0
    return std::max(0, finalScore);
}

std::string ScoreManager::formatTime(std::chrono::milliseconds duration) const {
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
    auto milliseconds = duration.count() % 1000;

    std::ostringstream oss;
    oss << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    if (milliseconds > 0) {
        oss << "." << std::setfill('0') << std::setw(3) << milliseconds;
    }

    return oss.str();
}
