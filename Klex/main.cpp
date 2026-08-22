/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/
#include "lexer.h"
#include "parser.h"

#include <iostream>
#include <vector>
#include <string>

int main() {

	std::string path = "readFile.kx";

    LEXER lexer(path, false);//file path | debug mode
    lexer.start();

    Parser parser(lexer.getTokens());

    auto ast = parser.parseExpression();
    printAST(ast);

	return 0;
}

/* TODO



*/