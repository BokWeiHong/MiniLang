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
    if (indent == -1) {
        result += type + "\n";
    } else {
        result += std::string(indent, ' ') + "|-- " + type + "\n";
    }

    for (const auto& child : children) {
        result += child->toString();
    }

    return result;
}

