#include "ASTNode.hpp"
#include <iostream>

// Constructor implementations
ASTNode::ASTNode(const std::string& type) : type(type) {}

ASTNode::ASTNode(const std::string& type, std::unique_ptr<ASTNode> child) 
    : type(type) {
    addChild(std::move(child));
}

ASTNode::ASTNode(const std::string& type, const std::string& value, std::unique_ptr<ASTNode> child) 
    : type(type) {
    // Assuming 'value' is used somewhere, but it's not in the class definition
    addChild(std::move(child));
}

ASTNode::ASTNode(const std::string& type, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) 
    : type(type) {
    addChild(std::move(left));
    addChild(std::move(right));
}

// Method implementations
void ASTNode::addChild(std::unique_ptr<ASTNode> child) {
    children.push_back(std::move(child));
}

std::string ASTNode::toString(int indentLevel) const {
    std::string indent(indentLevel, ' ');
    std::string result = indent + "Type: " + type + "\n";
    for (const auto& child : children) {
        result += child->toString(indentLevel + 2);
    }
    return result;
}

