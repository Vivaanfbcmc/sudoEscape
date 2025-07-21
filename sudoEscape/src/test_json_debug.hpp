#pragma once
#include <string>

// Global constants for JSON paths
#define LEVEL1_JSON_PATH "../data/levels/level1.json"
#define GAMESTATE_JSON_PATH "../data/gamestate.json"

// Debug function to load and print JSON info
void debugLoadJson(const std::string& jsonPath);
