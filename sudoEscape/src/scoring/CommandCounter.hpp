#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

enum class CommandStatus {
    SUCCESS,
    FAILURE,
    INVALID
};

struct CommandRecord {
    std::string command;
    CommandStatus status;
    std::chrono::steady_clock::time_point timestamp;

    CommandRecord(const std::string& cmd, CommandStatus stat)
        : command(cmd), status(stat), timestamp(std::chrono::steady_clock::now()) {}
};

class CommandCounter {
public:
    CommandCounter();
    ~CommandCounter();

    void recordCommand(const std::string& command, bool success);
    void recordCommand(const std::string& command, CommandStatus status);

    // Statistics
    int getTotalCommands() const;
    int getSuccessfulCommands() const;
    int getFailedCommands() const;
    int getInvalidCommands() const;

    // Command usage tracking
    std::unordered_map<std::string, int> getCommandUsage() const;
    std::string getMostUsedCommand() const;

    // History
    std::vector<CommandRecord> getCommandHistory() const;
    std::vector<std::string> getRecentCommands(int count = 10) const;

    // Efficiency metrics
    double getSuccessRate() const;
    std::chrono::milliseconds getAverageCommandTime() const;

    // Reset
    void reset();

private:
    std::vector<CommandRecord> commandHistory;
    std::unordered_map<std::string, int> commandUsage;
    std::chrono::steady_clock::time_point startTime;

    void updateCommandUsage(const std::string& command);
    std::string extractCommandName(const std::string& fullCommand) const;
};
