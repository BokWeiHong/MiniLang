#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <vector>
#include <memory>


class ASTNode {
public:
    ASTNode(const std::string& type);
    ASTNode(const std::string& type, const int& indent);
    std::string toString() const;
    void addChild(std::unique_ptr<ASTNode> child);
    
private:
    std::string type;
    int indent;
    std::vector<std::unique_ptr<ASTNode>> children;
};

#endif 