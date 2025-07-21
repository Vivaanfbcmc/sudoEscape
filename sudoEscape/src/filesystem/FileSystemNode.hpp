#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

enum class NodeType {
    FILE,
    DIRECTORY,
    SHORTCUT
};

struct FileSystemItem {
    std::string name;
    bool isDirectory;
    size_t size;
    std::string content;

    FileSystemItem(const std::string& n, bool isDir, size_t s = 0, const std::string& c = "")
        : name(n), isDirectory(isDir), size(s), content(c) {}
};

class FileSystemNode :  public std::enable_shared_from_this <FileSystemNode> {
public:
    FileSystemNode(const std::string& name, NodeType type, const std::string& content = "");
    ~FileSystemNode();

    // Node properties
    std::string getName() const { return name; }
    NodeType getType() const { return type; }
    std::string getContent() const { return content; }
    size_t getSize() const { return content.length(); }

    // Content management
    void setContent(const std::string& newContent);
    void appendContent(const std::string& additionalContent);

    // Directory operations
    void addChild(std::shared_ptr<FileSystemNode> child);
    void removeChild(const std::string& childName);
    std::shared_ptr<FileSystemNode> getChild(const std::string& childName);
    std::vector<std::shared_ptr<FileSystemNode>> getChildren() const;

    // Navigation
    std::shared_ptr<FileSystemNode> getParent() const { return parent.lock(); }
    void setParent(std::weak_ptr<FileSystemNode> parentNode) { parent = parentNode; }

    // Utility
    bool isDirectory() const { return type == NodeType::DIRECTORY; }
    bool isFile() const { return type == NodeType::FILE; }
    bool isShortcut() const { return type == NodeType::SHORTCUT; }

    std::vector<FileSystemItem> listItems(bool showHidden = false) const;

private:
    std::string name;
    NodeType type;
    std::string content;
    std::unordered_map<std::string, std::shared_ptr<FileSystemNode>> children;
    std::weak_ptr<FileSystemNode> parent;
};
