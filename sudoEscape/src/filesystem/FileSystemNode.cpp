#include "FileSystemNode.hpp"
#include <algorithm>

FileSystemNode::FileSystemNode(const std::string& name, NodeType type, const std::string& content)
    : name(name), type(type), content(content) {}

FileSystemNode::~FileSystemNode() = default;

void FileSystemNode::setContent(const std::string& newContent) {
    content = newContent;
}

void FileSystemNode::appendContent(const std::string& additionalContent) {
    content += additionalContent;
}

void FileSystemNode::addChild(std::shared_ptr<FileSystemNode> child) {
    if (child) {
        children[child->getName()] = child;
        child->setParent(shared_from_this());
    }
}

void FileSystemNode::removeChild(const std::string& childName) {
    auto it = children.find(childName);
    if (it != children.end()) {
        if (it->second) {
            it->second->setParent(std::weak_ptr<FileSystemNode>());
        }
        children.erase(it);
    }
}

std::shared_ptr<FileSystemNode> FileSystemNode::getChild(const std::string& childName) {
    auto it = children.find(childName);
    return (it != children.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<FileSystemNode>> FileSystemNode::getChildren() const {
    std::vector<std::shared_ptr<FileSystemNode>> result;
    for (const auto& pair : children) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<FileSystemItem> FileSystemNode::listItems(bool showHidden) const {
    std::vector<FileSystemItem> items;

    for (const auto& pair : children) {
        const auto& child = pair.second;

        // Skip hidden files unless requested
        if (!showHidden && child->getName().front() == '.') {
            continue;
        }

        items.emplace_back(
            child->getName(),
            child->isDirectory(),
            child->getSize(),
            child->getContent()
        );
    }

    // Sort items: directories first, then files
    std::sort(items.begin(), items.end(), [](const FileSystemItem& a, const FileSystemItem& b) {
        if (a.isDirectory != b.isDirectory) {
            return a.isDirectory; // Directories first
        }
        return a.name < b.name; // Alphabetical within same type
    });

    return items;
}
