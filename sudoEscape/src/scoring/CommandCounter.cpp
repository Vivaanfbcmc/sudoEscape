#include "CommandCounter.hpp"
#include <algorithm>
#include <sstream>

CommandCounter::CommandCounter() {
    reset();
}

CommandCounter::~CommandCounter() = default;

void CommandCounter::recordCommand(const std::string& command, bool success) {
    CommandStatus status = success ? CommandStatus::SUCCESS : CommandStatus::FAILURE;
    recordCommand(command, status);
}

void CommandCounter::recordCommand(const std::string& command, CommandStatus status) {
    commandHistory.emplace_back(command, status);
    updateCommandUsage(command);
}

int CommandCounter::getTotalCommands() const {
    return static_cast<int>(commandHistory.size());
}

int CommandCounter::getSuccessfulCommands() const {
    return static_cast<int>(std::count_if(commandHistory.begin(), commandHistory.end(),
        [](const CommandRecord& record) { return record.status == CommandStatus::SUCCESS; }));
}

int CommandCounter::getFailedCommands() const {
    return static_cast<int>(std::count_if(commandHistory.begin(), commandHistory.end(),
        [](const CommandRecord& record) { return record.status == CommandStatus::FAILURE; }));
}

int CommandCounter::getInvalidCommands() const {
    return static_cast<int>(std::count_if(commandHistory.begin(), commandHistory.end(),
        [](const CommandRecord& record) { return record.status == CommandStatus::INVALID; }));
}

std::unordered_map<std::string, int> CommandCounter::getCommandUsage() const {
    return commandUsage;
}

std::string CommandCounter::getMostUsedCommand() const {
    if (commandUsage.empty()) {
        return "";
    }

    auto maxElement = std::max_element(commandUsage.begin(), commandUsage.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    return maxElement->first;
}

std::vector<CommandRecord> CommandCounter::getCommandHistory() const {
    return commandHistory;
}

std::vector<std::string> CommandCounter::getRecentCommands(int count) const {
    std::vector<std::string> recent;
    int start = std::max(0, static_cast<int>(commandHistory.size()) - count);

    for (int i = start; i < static_cast<int>(commandHistory.size()); ++i) {
        recent.push_back(commandHistory[i].command);
    }

    return recent;
}

double CommandCounter::getSuccessRate() const {
    if (commandHistory.empty()) {
        return 0.0;
    }

    int successful = getSuccessfulCommands();
    return static_cast<double>(successful) / static_cast<double>(commandHistory.size()) * 100.0;
}

std::chrono::milliseconds CommandCounter::getAverageCommandTime() const {
    if (commandHistory.size() < 2) {
        return std::chrono::milliseconds(0);
    }

    auto totalTime = commandHistory.back().timestamp - commandHistory.front().timestamp;
    auto avgTime = totalTime / (commandHistory.size() - 1);

    return std::chrono::duration_cast<std::chrono::milliseconds>(avgTime);
}

void CommandCounter::reset() {
    commandHistory.clear();
    commandUsage.clear();
    startTime = std::chrono::steady_clock::now();
}

void CommandCounter::updateCommandUsage(const std::string& command) {
    std::string commandName = extractCommandName(command);
    commandUsage[commandName]++;
}

std::string CommandCounter::extractCommandName(const std::string& fullCommand) const {
    std::istringstream iss(fullCommand);
    std::string commandName;
    iss >> commandName;
    return commandName;
}
