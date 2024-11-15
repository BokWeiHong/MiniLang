#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <vector>
#include <memory>


class ASTNode {
public:
    // Constructor for ASTNode class
    ASTNode(const std::string& type);
    ASTNode(const std::string& type, const int& indent);
    // Method to convert the node to a string representation
    std::string toString() const;
    // Method to add a child to the node
    void addChild(std::unique_ptr<ASTNode> child);
    
private:
    std::string type;
    int indent;
    // Vector to store the children of the node
    std::vector<std::unique_ptr<ASTNode>> children;
};

#endif 