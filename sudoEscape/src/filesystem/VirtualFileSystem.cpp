#include "VirtualFileSystem.hpp"
#include "../utils/Logger.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

VirtualFileSystem::VirtualFileSystem() {
    reset();
}

VirtualFileSystem::~VirtualFileSystem() = default;

bool VirtualFileSystem::loadFromJson(const std::string& jsonFile) {
    try {
        std::ifstream file(jsonFile);
        if (!file.is_open()) {
            Logger::getInstance().log("Cannot open file: " + jsonFile);
            return false;
        }

        nlohmann::json j;
        file >> j;

        initializeFromLevel(j);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().log("Error loading JSON: " + std::string(e.what()));
        return false;
    }
}

bool VirtualFileSystem::saveToJson(const std::string& jsonFile) {
    try {
        std::ofstream file(jsonFile);
        if (!file.is_open()) {
            Logger::getInstance().log("Cannot create file: " + jsonFile);
            return false;
        }

        nlohmann::json j = nodeToJson(root);
        file << j.dump(4);

        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().log("Error saving JSON: " + std::string(e.what()));
        return false;
    }
}

void VirtualFileSystem::initializeFromLevel(const nlohmann::json& levelData) {
    reset();

    if (levelData.contains("locations")) {
        const auto& locations = levelData["locations"];

        for (const auto& [locationName, locationData] : locations.items()) {
            auto location = std::make_shared<FileSystemNode>(locationName, NodeType::DIRECTORY);
            root->addChild(location);

            createDirectoryStructure(location, locationData);
        }
    }

    // Set current directory to starting location
    std::string startLocation = "desktop";
    if (levelData.contains("level_info") && levelData["level_info"].contains("starting_location")) {
        startLocation = levelData["level_info"]["starting_location"];
    }

    changeDirectory(startLocation);
}

bool VirtualFileSystem::changeDirectory(const std::string& path) {
    if (path.empty()) {
        return false;
    }

    std::shared_ptr<FileSystemNode> target = nullptr;

    // Handle absolute paths
    if (path[0] == '/') {
        target = root;
        // Parse path components
        std::string remainingPath = path.substr(1);
        // For simplicity, we'll just handle single directory names
        if (!remainingPath.empty()) {
            target = root->getChild(remainingPath);
        }
    } else {
        // Handle relative paths
        if (path == "..") {
            target = currentDirectory->getParent();
            if (!target) {
                target = root;
            }
        } else {
            target = currentDirectory->getChild(path);
        }
    }

    if (target && target->isDirectory()) {
        directoryStack.push(currentDirectory);
        currentDirectory = target;
        return true;
    }

    return false;
}

bool VirtualFileSystem::goBack() {
    if (directoryStack.empty()) {
        return false;
    }

    currentDirectory = directoryStack.top();
    directoryStack.pop();
    return true;
}

std::string VirtualFileSystem::getCurrentPath() const {
    return getNodePath(currentDirectory);
}

std::vector<FileSystemItem> VirtualFileSystem::listCurrentDirectory(bool showHidden) const {
    return currentDirectory->listItems(showHidden);
}

std::string VirtualFileSystem::readFile(const std::string& filename) const {
    auto file = currentDirectory->getChild(filename);
    if (file && file->isFile()) {
        return file->getContent();
    }
    return "";
}

bool VirtualFileSystem::writeFile(const std::string& filename, const std::string& content) {
    auto file = currentDirectory->getChild(filename);
    if (file && file->isFile()) {
        file->setContent(content);
        return true;
    }
    return false;
}

bool VirtualFileSystem::createFile(const std::string& filename, const std::string& content) {
    if (currentDirectory->getChild(filename)) {
        return false; // File already exists
    }

    auto newFile = std::make_shared<FileSystemNode>(filename, NodeType::FILE, content);
    currentDirectory->addChild(newFile);
    return true;
}

bool VirtualFileSystem::deleteFile(const std::string& filename) {
    auto file = currentDirectory->getChild(filename);
    if (file) {
        currentDirectory->removeChild(filename);
        return true;
    }
    return false;
}

std::vector<std::string> VirtualFileSystem::findFiles(const std::string& pattern) const {
    std::vector<std::string> results;
    findFilesRecursive(root, pattern, results, "");
    return results;
}

bool VirtualFileSystem::fileExists(const std::string& filename) const {
    return currentDirectory->getChild(filename) != nullptr;
}

void VirtualFileSystem::printTree() const {
    std::cout << "File System Tree:\n";
    printTreeRecursive(root, "");
}

void VirtualFileSystem::reset() {
    root = std::make_shared<FileSystemNode>("root", NodeType::DIRECTORY);
    currentDirectory = root;
    while (!directoryStack.empty()) {
        directoryStack.pop();
    }

    initializeDefaultStructure();
}

void VirtualFileSystem::initializeDefaultStructure() {
    // Create desktop directory
    auto desktop = std::make_shared<FileSystemNode>("desktop", NodeType::DIRECTORY);
    root->addChild(desktop);

    // Create basic shortcuts on desktop
    auto myComputer = std::make_shared<FileSystemNode>("My Computer", NodeType::SHORTCUT, "System Information");
    auto fileExplorer = std::make_shared<FileSystemNode>("File Explorer", NodeType::SHORTCUT, "File Browser");

    desktop->addChild(myComputer);
    desktop->addChild(fileExplorer);

    currentDirectory = desktop;
}

void VirtualFileSystem::createDirectoryStructure(std::shared_ptr<FileSystemNode> parent, const nlohmann::json& locationData) {
    if (locationData.contains("items")) {
        for (const auto& item : locationData["items"]) {
            std::string name = item.value("name", "");
            std::string type = item.value("type", "file");
            std::string content = item.value("content", "");

            NodeType nodeType = (type == "folder") ? NodeType::DIRECTORY : 
                               (type == "shortcut") ? NodeType::SHORTCUT : NodeType::FILE;

            auto child = std::make_shared<FileSystemNode>(name, nodeType, content);
            parent->addChild(child);

            // If it's a directory and has nested items, create them recursively
            if (nodeType == NodeType::DIRECTORY && item.contains("items")) {
                createDirectoryStructure(child, item);
            }
        }
    }
}

void VirtualFileSystem::findFilesRecursive(std::shared_ptr<FileSystemNode> node, const std::string& pattern, 
                                         std::vector<std::string>& results, const std::string& currentPath) const {
    if (!node) return;

    std::string nodePath = currentPath + "/" + node->getName();

    if (node->getName().find(pattern) != std::string::npos) {
        results.push_back(nodePath);
    }

    if (node->isDirectory()) {
        for (const auto& child : node->getChildren()) {
            findFilesRecursive(child, pattern, results, nodePath);
        }
    }
}

std::string VirtualFileSystem::getNodePath(std::shared_ptr<FileSystemNode> node) const {
    if (!node || node == root) {
        return "/";
    }

    std::string path = "";
    std::shared_ptr<FileSystemNode> current = node;

    while (current && current != root) {
        path = "/" + current->getName() + path;
        current = current->getParent();
    }

    return path.empty() ? "/" : path;
}

void VirtualFileSystem::printTreeRecursive(std::shared_ptr<FileSystemNode> node, const std::string& prefix) const {
    if (!node) return;

    std::cout << prefix << node->getName();
    if (node->isDirectory()) {
        std::cout << "/";
    }
    std::cout << "\n";

    if (node->isDirectory()) {
        for (const auto& child : node->getChildren()) {
            printTreeRecursive(child, prefix + "  ");
        }
    }
}

nlohmann::json VirtualFileSystem::nodeToJson(std::shared_ptr<FileSystemNode> node) const {
    nlohmann::json j;

    j["name"] = node->getName();
    j["type"] = node->isDirectory() ? "directory" : "file";
    j["content"] = node->getContent();

    if (node->isDirectory()) {
        j["children"] = nlohmann::json::array();
        for (const auto& child : node->getChildren()) {
            j["children"].push_back(nodeToJson(child));
        }
    }

    return j;
}

std::shared_ptr<FileSystemNode> VirtualFileSystem::jsonToNode(const nlohmann::json& j, const std::string& name) {
    NodeType type = (j.value("type", "file") == "directory") ? NodeType::DIRECTORY : NodeType::FILE;
    std::string content = j.value("content", "");

    auto node = std::make_shared<FileSystemNode>(name, type, content);

    if (j.contains("children")) {
        for (const auto& child : j["children"]) {
            std::string childName = child.value("name", "");
            auto childNode = jsonToNode(child, childName);
            node->addChild(childNode);
        }
    }

    return node;
}
