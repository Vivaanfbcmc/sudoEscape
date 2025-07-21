



#include "test_json_debug.hpp"
#include <iostream>
#include <fstream>
#include "../../dependencies/include/nlohmann/json.hpp"

void debugLoadJson(const std::string& jsonPath) {
    std::cout << "[DEBUG] Attempting to open: " << jsonPath << std::endl;
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << jsonPath << std::endl;
        return;
    }
    try {
        nlohmann::json j;
        file >> j;
        std::cout << "[DEBUG] Successfully loaded JSON. Top-level keys:" << std::endl;
        for (auto it = j.begin(); it != j.end(); ++it) {
            std::cout << "  " << it.key() << std::endl;
        }
        // Example: print the goal if it exists
        if (j.contains("level_info") && j["level_info"].contains("goal")) {
            std::cout << "Goal: " << j["level_info"]["goal"] << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Exception while parsing JSON: " << e.what() << std::endl;
    }
}
