#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../game/GameState.hpp"
#include "../filesystem/VirtualFileSystem.hpp"

class Commands {
public:
    Commands(GameState& gameState, VirtualFileSystem& fileSystem);
    ~Commands();

    // Navigation commands
    bool cd(const std::string& directory);
    bool ls(const std::vector<std::string>& args = {});
    bool pwd();
    bool back();
    bool open(const std::string& item);

    // Analysis commands
    bool cat(const std::string& filename);
    bool head(const std::string& filename, int lines = 10);
    bool tail(const std::string& filename, int lines = 10);
    bool grep(const std::string& pattern, const std::string& filename);
    bool strings(const std::string& filename);
    bool xxd(const std::string& filename);

    // Decoding commands
    bool base64(const std::string& input);
    bool rot13(const std::string& input);
    bool decode(const std::string& input);

    // CRUD commands
    bool touch(const std::string& filename);
    bool create(const std::string& filename, const std::string& content);
    bool edit(const std::string& filename);
    bool rm(const std::string& filename);
    bool deleteFile(const std::string& filename);

    // Utility commands
    bool help();
    bool clear();
    bool count();
    bool history();

    // Special commands
    bool examine(const std::string& item);
    bool find(const std::string& name);
    bool solve(const std::string& code);

private:
    GameState& gameState;
    VirtualFileSystem& fileSystem;
    std::vector<std::string> commandHistory;

    std::string base64Decode(const std::string& input);
    std::string rot13Decode(const std::string& input);
    std::string caesarDecode(const std::string& input, int shift);
    bool isBase64(const std::string& input);
    void addToHistory(const std::string& command);
};
