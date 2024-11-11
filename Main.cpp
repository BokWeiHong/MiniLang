#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Lexer.hpp"
#include "Token.hpp"

int main(int argc, char* argv[]){
    if (argc != 2){
        std::cout << "Incorrect usage. Correct usage is ..." << std::endl;
        std::cout << "minilang input.lang" << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
    if (filename.substr(filename.find_last_of(".") + 1) != "lang") {
        std::cout << "Error: The file must have a .lang extension" << std::endl;
        return EXIT_FAILURE;
    }

    std::string sourceCode;
    std::fstream input(argv[1], std::ios::in);
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file" << std::endl;
        return 1;
    }

    std::stringstream contents_stream;
    contents_stream << input.rdbuf();
    sourceCode = contents_stream.str();

    std::cout << "\n" <<sourceCode << "\n" << std::endl;

    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "Tokens:" << std::endl;
    for (const Token& token : tokens) {
        std::cout << token.getType() << ": " << token.getValue() << std::endl;
    }

    return EXIT_SUCCESS;
}