#include "ASTNode.hpp"
#include <iostream>

// Constructor implementations
ASTNode::ASTNode(const std::string& type) : type(type) {}

ASTNode::ASTNode(const std::string& type, const int& indent) 
    : type(type), indent(indent) {}

void ASTNode::addChild(std::unique_ptr<ASTNode> child) {
    children.push_back(std::move(child));
}

std::string ASTNode::toString() const {
    std::string result;
    // Check if the node has no indentation
    if (indent == -1) {
        result += type + "\n";
    } else {
        // Add the type of the node to the result with indentation
        result += std::string(indent * 3, ' ') + "|- " + type + "\n";
    }
    // Loop through the children of the node
    for (const auto& child : children) {
        // Recursively convert each child to a string and append it to the result
        result += child->toString();
    }

    return result;
}

