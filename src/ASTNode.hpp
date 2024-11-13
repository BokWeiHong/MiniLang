#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    ASTNode(const std::string& type);
    ASTNode(const std::string& type, std::unique_ptr<ASTNode> child);
    ASTNode(const std::string& type, const std::string& value, std::unique_ptr<ASTNode> child);
    ASTNode(const std::string& type, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);
    void addChild(std::unique_ptr<ASTNode> child);
    std::string toString(int indentLevel) const;

private:
    std::string type;
    std::vector<std::unique_ptr<ASTNode>> children;
};

#endif 