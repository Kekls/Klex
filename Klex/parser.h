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
	virtual void print(int depth = 0) const = 0;
};

struct numberNode : public Node {
	std::string name = "NumberNode";
	long long value;

	numberNode(long long value) : value(value) {}
	void print(int depth = 0) const override;
};

struct operatorNode : public Node {
	std::string name = "OperatorNode";
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;
	TOKEN_TYPE type;

	operatorNode(std::unique_ptr<Node> left, TOKEN_TYPE type): left(std::move(left)), right(nullptr), type(type) {}
	void print(int depth = 0) const override;
};

class Parser {
private:
	std::vector<TOKEN> tokens;//stores all tokens
	size_t current = 0;
	bool debug;//on/off of debug mode

public:
	Parser(std::vector<TOKEN> tokens, bool debug);

	TOKEN peek();
	TOKEN advance();
	bool checkAheadTokenType(TOKEN_TYPE type);
	bool checkTokenType(TOKEN_TYPE type);

	void start();
	std::unique_ptr<Node> parseExpresion();
};