#pragma once
#include <string>
#include <chrono>
#include <memory>
#include "CommandCounter.hpp"

enum class EfficiencyTier {
    PLATINUM,
    GOLD,
    SILVER,
    BRONZE
};

class ScoreManager {
public:
    ScoreManager();
    ~ScoreManager();

    // Command tracking
    void recordCommand(const std::string& command, bool success);
    int getCommandCount() const;

    // Score calculation
    int getCurrentScore() const;
    int getBaseScore() const { return BASE_SCORE; }
    int getCommandPenalty() const;
    int getDiscoveryBonus() const;
    int getHintPenalty() const;
    int getTimeBonus() const;

    // Efficiency tracking
    EfficiencyTier getEfficiencyTier() const;
    double getEfficiencyMultiplier() const;
    std::string getEfficiencyTierName() const;

    // Game events
    void recordDiscovery();
    void recordHint();
    void setStartTime(std::chrono::steady_clock::time_point time);
    void setEndTime(std::chrono::steady_clock::time_point time);

    // Statistics
    int getDiscoveryCount() const { return discoveryCount; }
    int getHintCount() const { return hintCount; }
    std::chrono::milliseconds getElapsedTime() const;
    double getSuccessRate() const;

    // Display
    void displayCurrentScore() const;
    void displayFinalScore() const;
    void displayDetailedStats() const;

    // Reset
    void reset();

private:
    static const int BASE_SCORE = 1000;
    static const int COMMAND_PENALTY = 2;
    static const int DISCOVERY_BONUS = 25;
    static const int HINT_PENALTY = 50;
    static const int MAX_TIME_BONUS = 500;

    // Efficiency tier thresholds
    static const int PLATINUM_THRESHOLD = 25;
    static const int GOLD_THRESHOLD = 35;
    static const int SILVER_THRESHOLD = 50;

    // Efficiency multipliers
    static const double PLATINUM_MULTIPLIER;
    static const double GOLD_MULTIPLIER;
    static const double SILVER_MULTIPLIER;
    static const double BRONZE_MULTIPLIER;

    std::unique_ptr<CommandCounter> commandCounter;
    int discoveryCount;
    int hintCount;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    bool gameCompleted;

    int calculateTimeBonus() const;
    int calculateFinalScore() const;
    std::string formatTime(std::chrono::milliseconds duration) const;
};
