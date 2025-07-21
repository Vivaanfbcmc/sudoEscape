#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <nlohmann/json.hpp>
#include "FileSystemNode.hpp"

class VirtualFileSystem {
public:
    VirtualFileSystem();
    ~VirtualFileSystem();

    // Initialization
    bool loadFromJson(const std::string& jsonFile);
    bool saveToJson(const std::string& jsonFile);
    void initializeFromLevel(const nlohmann::json& levelData);

    // Navigation
    bool changeDirectory(const std::string& path);
    bool goBack();
    std::string getCurrentPath() const;
    std::vector<FileSystemItem> listCurrentDirectory(bool showHidden = false) const;

    // File operations
    std::string readFile(const std::string& filename) const;
    bool writeFile(const std::string& filename, const std::string& content);
    bool createFile(const std::string& filename, const std::string& content);
    bool deleteFile(const std::string& filename);

    // Search operations
    std::vector<std::string> findFiles(const std::string& pattern) const;
    bool fileExists(const std::string& filename) const;

    // Utility
    void printTree() const;
    void reset();

private:
    std::shared_ptr<FileSystemNode> root;
    std::shared_ptr<FileSystemNode> currentDirectory;
    std::stack<std::shared_ptr<FileSystemNode>> directoryStack;

    void initializeDefaultStructure();
    void createDirectoryStructure(std::shared_ptr<FileSystemNode> parent, const nlohmann::json& locationData);
    void findFilesRecursive(std::shared_ptr<FileSystemNode> node, const std::string& pattern, 
                           std::vector<std::string>& results, const std::string& currentPath) const;
    std::string getNodePath(std::shared_ptr<FileSystemNode> node) const;
    void printTreeRecursive(std::shared_ptr<FileSystemNode> node, const std::string& prefix) const;

    // JSON conversion helpers
    nlohmann::json nodeToJson(std::shared_ptr<FileSystemNode> node) const;
    std::shared_ptr<FileSystemNode> jsonToNode(const nlohmann::json& j, const std::string& name);
};
