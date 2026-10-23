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

	try {
		std::string path = "readFile.kx";

		LEXER lexer(path, true);//file path | debug mode
		lexer.start();

		Parser parser(lexer.getTokens(), true);//tokens | debug mode
		parser.start();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}

	

	return 0;
}

/* TODO

add if xD


--- NOTES ---
PARSER::numberNode & identifierNode print -> maybe shorten?
PARSER::parseComparison -> think abaut the relativity and eaqulity ( a>b==c and c==b>a case)
*/