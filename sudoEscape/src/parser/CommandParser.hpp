#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

enum class CommandType {
    NAVIGATION,
    ANALYSIS,
    DECODING,
    CRUD,
    UTILITY,
    SPECIAL,
    UNKNOWN
};

struct CommandResult {
    bool isValid;
    CommandType type;
    std::string command;
    std::string primaryArg;
    std::vector<std::string> args;
    std::string fullCommand;

    CommandResult() : isValid(false), type(CommandType::UNKNOWN) {}
};

class CommandParser {
public:
    CommandParser();
    ~CommandParser();

    CommandResult parseCommand(const std::string& input);
    std::string getHelpText() const;
    bool isValidCommand(const std::string& command) const;

private:
    std::unordered_map<std::string, CommandType> commandTypes;
    std::unordered_map<CommandType, std::vector<std::string>> typeCommands;

    void initializeCommands();
    std::vector<std::string> tokenize(const std::string& input);
    std::string trim(const std::string& str);
    std::string toLowerCase(const std::string& str) const;
};
