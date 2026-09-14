/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/

#include "parser.h"

void numberNode::print(int depth) const {
	std::string indent(depth * 2, ' ');
	std::cout << indent << "VALUE: " << value << '\n';
}

void operatorNode::print(int depth) const {
	std::string indent(depth * 2, ' ');

	std::cout << indent << "OPERATOR: " << LEXER::tokenToString(type) << '\n';

	if (right) {
		right->print(depth + 1);
	}
	else {
		std::cout << indent << "  Right: nullptr\n";
	}

	if (left) {
		left->print(depth + 1);
	}
	else {
		std::cout << indent << "  Left: nullptr\n";
	}
}

Parser::Parser(std::vector<TOKEN> tokens, bool debug) {
	this->tokens = std::move(tokens);
	this->debug = debug;
}

TOKEN Parser::peek() {
	if (current >= tokens.size()) {
		return TOKEN{ TOKEN_TYPE::END_OF_FILE };
	}
	return tokens[current];
}

TOKEN Parser::advance() {
	if (current >= tokens.size()) {
		return TOKEN{ TOKEN_TYPE::END_OF_FILE };
	}

	current++;
	return tokens[current-1];
}

bool Parser::checkAheadTokenType(TOKEN_TYPE type) {
	if (current + 1 >= tokens.size()) {
		return true;
	}

	return tokens[current + 1].type == type;
}

bool Parser::checkTokenType(TOKEN_TYPE type) {
	return peek().type == type;
}

void Parser::start() {
	std::unique_ptr<Node> root = parseExpresion();

	if (root != nullptr) {
		std::cout << "\n\n--- Parser DUMP ---\n\n";
		root->print();
	}
	else {
		std::cout << "Parser gave empty AST tree\n";
	}

}

std::unique_ptr<Node> Parser::parseExpresion() {
	TOKEN memory;
	std::unique_ptr<operatorNode> root;
	bool afterFirstOperator = false;
	bool memoryEmpty = true;

	while (true) {
		if (!checkTokenType(TOKEN_TYPE::NUMBER) && !afterFirstOperator && !checkTokenType(TOKEN_TYPE::END_OF_FILE)) {

			root = std::make_unique<operatorNode>(std::make_unique<numberNode>(std::stoll(memory.value)), advance().type);
			afterFirstOperator = true;
		}
		else {
			if (afterFirstOperator) {
				if (!root) {
					std::cout << "CRITICAL ERROR: afterFirstOperator is true, but root is nullptr!\n";
					return nullptr;
				}

				if (!checkAheadTokenType(TOKEN_TYPE::NUMBER) && !checkAheadTokenType(TOKEN_TYPE::END_OF_FILE)) {
					memory = advance();

					root->right = std::make_unique<numberNode>(std::stoll(memory.value));
					std::unique_ptr<operatorNode> temp;

					temp = std::make_unique<operatorNode>(std::move(root), advance().type);
					root = std::move(temp);
				}
				else {
					memory = peek();
					root->right = std::move(std::make_unique<numberNode>(std::stoll(memory.value)));

					return root;
				}
			}
			else {
				if (peek().type == TOKEN_TYPE::END_OF_FILE) {
					if (memoryEmpty) {
						return nullptr;
					}
					else {
						return std::make_unique<numberNode>(std::stoll(memory.value));
					}
				}
				else {
					memory = advance();
					memoryEmpty = false;
				}
			}
		}
	}
}