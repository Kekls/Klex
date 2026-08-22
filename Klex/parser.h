/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/
#pragma once

#include "lexer.h"
#include <vector>
#include <memory>

struct Node {
	virtual ~Node() = default;
};

struct NumberNode : Node {
	float value;

	NumberNode(float value) : value(value) {}
};

struct BinaryNode : Node {
	TOKEN_TYPE op;

	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;

	BinaryNode(
		TOKEN_TYPE op,
		std::unique_ptr<Node> left,
		std::unique_ptr<Node> right) :
		op(op),
		left(std::move(left)),
		right(std::move(right))
	{}
};

class Parser {
private:
	std::vector<TOKEN> tokens;
	size_t current = 0;

public:
	Parser(std::vector<TOKEN> tokens);

	std::unique_ptr<Node> parseExpression();
};

void printIndent(int indent);
void printAST(const std::unique_ptr<Node>& node, int indent = 0);
