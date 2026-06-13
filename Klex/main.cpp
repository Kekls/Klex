/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/
#include "lexer.h"

#include <iostream>
#include <vector>
#include <string>

int main() {

	std::string path = "readFile.kx";

    LEXER lexer(path, true);//file path | debug mode
    lexer.start();

    std::vector<TOKEN> tokens = lexer.getTokens();

	return 0;
}

/* TODO

* Comment line needing \n to be categorized as COMMENT_LINE (if not it is IDENTIFIER)

*/