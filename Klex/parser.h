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

struct programNode : public Node {
	std::string name = "programNode";
	std::vector<std::unique_ptr<Node>> roots;

	void print(int depth = 0) const override;
};

struct numberNode : public Node {
	std::string name = "numberNode";
	std::variant<long long, double> value;

	numberNode(long long value) : value(value) {}
	numberNode(double value) : value(value) {}

	void print(int depth = 0) const override;
};

struct identifierNode : public Node {
	std::string name = "identifierNode";
	std::string value;

	identifierNode(std::string value) : value(value) {}

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

struct variableDeclarationNode : public Node {
	std::string name = "variableDeclarationNode";
	std::string value;

	std::unique_ptr<Node> inicializtion;

	variableDeclarationNode(std::string value) : value(value) {}

	void print(int depth = 0) const override;
};

class Parser {
private:
	std::vector<TOKEN> tokens;
	size_t current = 0;
	bool debug = false;
	TOKEN memory;

	std::unique_ptr<programNode> program;

public:
	Parser(std::vector<TOKEN> tokens, bool debug);

	TOKEN peek();
	TOKEN advance();
	bool checkTokenType(TOKEN_TYPE type);
	void skipStatement();
	TOKEN expect(TOKEN_TYPE type, const std::string& what);

	void start();

	std::unique_ptr<Node> parseStatement();
	std::unique_ptr<Node> parseAssigment();
	std::unique_ptr<Node> parseComparison();
	std::unique_ptr<Node> parseExpresion();
	std::unique_ptr<Node> parseTerm();
	std::unique_ptr<Node> parseFactor();
};