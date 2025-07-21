#include "CommandParser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include<iomanip>

CommandParser::CommandParser() {
    initializeCommands();
}

CommandParser::~CommandParser() = default;

CommandResult CommandParser::parseCommand(const std::string& input) {
    CommandResult result;

    if (input.empty()) {
        return result;
    }

    result.fullCommand = input;

    // Tokenize the input
    std::vector<std::string> tokens = tokenize(input);
    if (tokens.empty()) {
        return result;
    }

    // Get the main command (first token)
    std::string mainCommand = toLowerCase(tokens[0]);
    result.command = mainCommand;

    // Check if it's a valid command
    auto it = commandTypes.find(mainCommand);
    if (it == commandTypes.end()) {
        return result; // isValid remains false
    }

    result.isValid = true;
    result.type = it->second;

    // Parse arguments
    for (size_t i = 1; i < tokens.size(); ++i) {
        result.args.push_back(tokens[i]);
    }

    // Set primary argument (usually the first argument)
    if (!result.args.empty()) {
        result.primaryArg = result.args[0];
    }

    return result;
}

std::string CommandParser::getHelpText() const {
    std::stringstream help;
    help << "\n=== AVAILABLE COMMANDS ===\n";

    help << "\nNavigation Commands:\n";
    help << "  cd <directory>    - Change to directory\n";
    help << "  ls                - List contents\n";
    help << "  ls -la            - List all contents (including hidden)\n";
    help << "  pwd               - Show current directory\n";
    help << "  back              - Go back to previous directory\n";
    help << "  open <item>       - Open file or shortcut\n";

    help << "\nFile Analysis Commands:\n";
    help << "  cat <file>        - Display file contents\n";
    help << "  head <file>       - Show first lines of file\n";
    help << "  tail <file>       - Show last lines of file\n";
    help << "  grep <pattern> <file> - Search for pattern in file\n";
    help << "  strings <file>    - Extract text from binary file\n";
    help << "  xxd <file>        - Hexdump of file\n";

    help << "\nDecoding Commands:\n";
    help << "  base64 <string>   - Decode Base64 string\n";
    help << "  rot13 <string>    - Decode ROT13 cipher\n";
    help << "  decode <string>   - Auto-detect and decode\n";

    help << "\nFile Operations (CRUD):\n";
    help << "  touch <file>      - Create empty file\n";
    help << "  create <file> <content> - Create file with content\n";
    help << "  edit <file>       - Edit file contents\n";
    help << "  rm <file>         - Remove file\n";
    help << "  delete <file>     - Delete file\n";

    help << "\nUtility Commands:\n";
    help << "  help              - Show this help\n";
    help << "  clear             - Clear screen\n";
    help << "  count             - Show command statistics\n";
    help << "  history           - Show command history\n";

    help << "\nSpecial Commands:\n";
    help << "  examine <item>    - Examine item closely\n";
    help << "  find <name>       - Find files by name\n";
    help << "  solve <code>      - Submit solution code\n";
    help << "  pause             - Access pause menu\n";

    help << "\n========================\n";

    return help.str();
}

bool CommandParser::isValidCommand(const std::string& command) const {
    std::string lowerCommand = toLowerCase(command);
    return commandTypes.find(lowerCommand) != commandTypes.end();
}

void CommandParser::initializeCommands() {
    // Navigation commands
    commandTypes["cd"] = CommandType::NAVIGATION;
    commandTypes["ls"] = CommandType::NAVIGATION;
    commandTypes["pwd"] = CommandType::NAVIGATION;
    commandTypes["back"] = CommandType::NAVIGATION;
    commandTypes["open"] = CommandType::NAVIGATION;

    // Analysis commands
    commandTypes["cat"] = CommandType::ANALYSIS;
    commandTypes["head"] = CommandType::ANALYSIS;
    commandTypes["tail"] = CommandType::ANALYSIS;
    commandTypes["grep"] = CommandType::ANALYSIS;
    commandTypes["strings"] = CommandType::ANALYSIS;
    commandTypes["xxd"] = CommandType::ANALYSIS;

    // Decoding commands
    commandTypes["base64"] = CommandType::DECODING;
    commandTypes["rot13"] = CommandType::DECODING;
    commandTypes["decode"] = CommandType::DECODING;

    // CRUD commands
    commandTypes["touch"] = CommandType::CRUD;
    commandTypes["create"] = CommandType::CRUD;
    commandTypes["edit"] = CommandType::CRUD;
    commandTypes["rm"] = CommandType::CRUD;
    commandTypes["delete"] = CommandType::CRUD;

    // Utility commands
    commandTypes["help"] = CommandType::UTILITY;
    commandTypes["clear"] = CommandType::UTILITY;
    commandTypes["count"] = CommandType::UTILITY;
    commandTypes["history"] = CommandType::UTILITY;

    // Special commands
    commandTypes["examine"] = CommandType::SPECIAL;
    commandTypes["find"] = CommandType::SPECIAL;
    commandTypes["solve"] = CommandType::SPECIAL;

    // Build reverse mapping
    for (const auto& pair : commandTypes) {
        typeCommands[pair.second].push_back(pair.first);
    }
}

// Fix for parsing quoted strings like "My Computer"
// Updated CommandParser::tokenize()

std::vector<std::string> CommandParser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string current;
    bool inQuotes = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == '"') {
            inQuotes = !inQuotes; // toggle quote state
            continue;
        }

        if (std::isspace(c) && !inQuotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}


std::string CommandParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string CommandParser::toLowerCase(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
