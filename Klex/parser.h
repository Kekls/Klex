/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/
#pragma once

#include "lexer.h"
#include <vector>
#include <memory>
#include <variant>

struct Node {
	virtual ~Node() = default;
	virtual void print(int depth = 0) const = 0;
};

struct numberNode : public Node {
	std::string name = "NumberNode";
	std::variant<long long, double> value;

	numberNode(long long value) : value(value) {}
	numberNode(double value) : value(value) {}

	void print(int depth = 0) const override;
};

struct operatorNode : public Node {
	std::string name = "OperatorNode";
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;
	TOKEN_TYPE type;

	operatorNode(TOKEN_TYPE type,
		std::unique_ptr<Node> left = nullptr,
		std::unique_ptr<Node> right = nullptr)
		: left(std::move(left)), right(std::move(right)), type(type) {
	}

	void print(int depth = 0) const override;
};

struct unaryOperatorNode : public Node {
	std::string name = "unaryOperatorNode";
	std::unique_ptr<Node> next;
	TOKEN_TYPE type;

	unaryOperatorNode(TOKEN_TYPE type,
		std::unique_ptr<Node> next = nullptr)
		: next(std::move(next)), type(type) {
	}

	void print(int depth = 0) const override;
};

class Parser {
private:
	std::vector<TOKEN> tokens;//stores all tokens
	size_t current = 0;
	bool debug;//on/off of debug mode

	TOKEN memory;
	std::unique_ptr<Node> root;

public:
	Parser(std::vector<TOKEN> tokens, bool debug);

	TOKEN peek();
	TOKEN advance();
	bool checkTokenType(TOKEN_TYPE type);
	TOKEN expect(TOKEN_TYPE type, const std::string& what);

	void start();

	std::unique_ptr<Node> parseExpresion();
	std::unique_ptr<Node> parseTerm();
	std::unique_ptr<Node> parseFactor();
};