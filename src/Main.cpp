#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Lexer.hpp"
#include "Token.hpp"
#include "ASTNode.hpp"
#include "Parser.hpp"

int main(int argc, char* argv[]){
    // Check if the correct number of arguments are provided
    if (argc != 2){
        std::cout << "Incorrect usage. Correct usage is ..." << std::endl;
        std::cout << "minilang input.lang" << std::endl;
        return EXIT_FAILURE;
    }

    // Check if the file has a .lang extension
    std::string filename = argv[1];
    if (filename.substr(filename.find_last_of(".") + 1) != "lang") {
        std::cout << "Error: The file must have a .lang extension" << std::endl;
        return EXIT_FAILURE;
    }

    // Read the source code from the file
    std::string sourceCode;
    std::fstream input(argv[1], std::ios::in);
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file" << std::endl;
        return 1;
    }

    // Read the contents of the file into a string
    std::stringstream contents_stream;
    contents_stream << input.rdbuf();
    sourceCode = contents_stream.str();

    std::cout << "\n" <<sourceCode << "\n" << std::endl;

    // Tokenize the source code
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tokens:" << std::endl;
    for (const Token& token : tokens) {
        std::cout << token.getType() << ": " << token.getValue() << std::endl;
    }

    // Parse the tokens into an AST
    Parser parser(tokens);
    std::unique_ptr<ASTNode> root = parser.parse();
    std::cout << "\n";
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Syntax Tree:" << std::endl;
    std::cout << root->toString() << std::endl; 
    std::cout << "----------------------------------------" << std::endl;

    return EXIT_SUCCESS;
}