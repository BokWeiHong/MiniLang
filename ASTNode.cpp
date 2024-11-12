#include <iostream>

#include "ASTNode.hpp"

ASTNode::ASTNode(const std::string& type) : type(type) {}

ASTNode::ASTNode(const std::string& type, std::unique_ptr<ASTNode> child) {
    this->type = type;
    children.push_back(std::move(child));
}
 
void ASTNode::addChild(std::unique_ptr<ASTNode> child) {
    children.push_back(std::move(child));
}

std::string ASTNode::toString(int indentLevel) const {
    std::string indent(indentLevel * 2, ' ');
    std::string result = indent + type + "\n";
    for (const auto& child : children) {
        result += child->toString(indentLevel + 1);
    }
    return result;
}

