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

    LEXER lexer(path, true);//file path | debug mode
    lexer.start();

	Parser parser(lexer.getTokens(), true);
	parser.start();

	return 0;
}

/* TODO

add if

*/