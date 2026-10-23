/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/

#include "parser.h"

void programNode::print(int depth) const {
	for (const std::unique_ptr<Node>& rootNode : roots) {
		if (rootNode) {
			rootNode->print(depth + 1);
			std::cout << "\n     --- + --- \n\n";
		}
		else
			std::cout << "nothing to print!\n\n     --- + --- \n\n";
	}
}

void numberNode::print(int depth) const {//maybe can shorten
	std::string indent(depth * 2, ' ');
	std::cout << indent << "VALUE: ";
	
	std::visit([](auto&& arg) {
		std::cout << arg << '\n';
		}, this->value);
}

void identifierNode::print(int depth) const {//maybe can shorten
	std::string indent(depth * 2, ' ');
	std::cout << indent << "IDENTIFIER: " << this->value << std::endl;
}

void operatorNode::print(int depth) const {
	std::string indent(depth * 2, ' ');

	std::cout << indent << "OPERATOR: " << LEXER::tokenToString(type) << '\n';

	if (left) {
		left->print(depth + 1);
	}
	else {
		std::cout << indent << "  Left: nullptr\n";
	}

	if (right) {
		right->print(depth + 1);
	}
	else {
		std::cout << indent << "  Right: nullptr\n";
	}
}

void variableDeclarationNode::print(int depth) const {
	std::string indent(depth * 2, ' ');

	std::cout << indent << "VARIABLE: " << value << '\n';

	if (inicializtion) {
		inicializtion->print(depth + 1);
	}
	else {
		std::cout << indent << "  Inicializtion: nullptr\n";
	}
}

void unaryOperatorNode::print(int depth) const {
	std::string indent(depth * 2, ' ');

	std::cout << indent << "UNARY_OPERATOR: " << LEXER::tokenToString(type) << '\n';

	if (next) {
		next->print(depth + 1);
	}
	else {
		std::cout << indent << " Next: nullptr \n";
	}
}

Parser::Parser(std::vector<TOKEN> tokens, bool debug) {
	this->tokens = std::move(tokens);
	this->debug = debug;
	this->program = std::make_unique<programNode>();
}

TOKEN Parser::peek() {
	size_t tempCurrent = current;

	if (tempCurrent >= tokens.size()) {
		return TOKEN{ TOKEN_TYPE::END_OF_FILE };
	}

	TOKEN token = tokens[tempCurrent];

	while (token.type == TOKEN_TYPE::NEW_LINE) {
		tempCurrent++;
		if (tempCurrent >= tokens.size()) {
			return TOKEN{ TOKEN_TYPE::END_OF_FILE };
		}
		token = tokens[tempCurrent];
	}

	return token;
}


TOKEN Parser::advance() {
	if (current >= tokens.size()) {
		return TOKEN{ TOKEN_TYPE::END_OF_FILE };
	}

	TOKEN token = tokens[current];
	current++;

	while (token.type == TOKEN_TYPE::NEW_LINE) {
		if (current >= tokens.size()) {
			return TOKEN{ TOKEN_TYPE::END_OF_FILE };
		}
		token = tokens[current];
		current++;
	}

	return token;
}

bool Parser::checkTokenType(TOKEN_TYPE type) {
	return peek().type == type;
}

void Parser::skipStatement() {
	if (current >= tokens.size()) return;

	while (!checkTokenType(TOKEN_TYPE::SEMICOLON) && !checkTokenType(TOKEN_TYPE::END_OF_FILE)) {
		current++;
	}

	if (checkTokenType(TOKEN_TYPE::SEMICOLON)) {
		current++;
	}
}

TOKEN Parser::expect(TOKEN_TYPE type, const std::string& what) {
	if (!checkTokenType(type)) {
		TOKEN t = peek();
		throw std::runtime_error("Expected " + what + " at line " + std::to_string(t.line) +
			", column " + std::to_string(t.column));
	}
	return advance();
}

void Parser::start() {
	while (!checkTokenType(TOKEN_TYPE::END_OF_FILE)) {
		program->roots.push_back(parseStatement());;
		current++;
	}

	if (debug) {
		std::cout << "\n ---PARSER DUMP--- \n\n";
		if (!program->roots.empty())
			program->print();
		else
			std::cout << "nothing to print!\n";
	}
}

std::unique_ptr<Node> Parser::parseStatement() {
	std::unique_ptr<Node> statementRoot;

	switch (peek().type) {
	case TOKEN_TYPE::VAR: {
		advance();
		std::unique_ptr<variableDeclarationNode> temp = std::make_unique<variableDeclarationNode>(expect(TOKEN_TYPE::IDENTIFIER, "identifier").value);
		if (!checkTokenType(TOKEN_TYPE::SEMICOLON) && !checkTokenType(TOKEN_TYPE::END_OF_FILE)) {
			expect(TOKEN_TYPE::ASSIGN, "=");
			temp->inicializtion = parseComparison();
		}
		statementRoot = std::move(temp);
		break;
	}
	case TOKEN_TYPE::IF:
		break;
	case TOKEN_TYPE::DO:
		break;
	case TOKEN_TYPE::WHILE:
		break;
	case TOKEN_TYPE::NUMBER:
		skipStatement();
		break;
	default:
		statementRoot = parseAssigment();
	}

	return std::move(statementRoot);
}

std::unique_ptr<Node> Parser::parseAssigment() {
	std::unique_ptr<Node> assigmentRoot;

	assigmentRoot = parseComparison();

	while (checkTokenType(TOKEN_TYPE::ASSIGN)) {
		std::unique_ptr<operatorNode> temp;
		temp = std::make_unique<operatorNode>(advance().type, std::move(assigmentRoot));

		temp->right = parseComparison();
		assigmentRoot = std::move(temp);
	}

	return std::move(assigmentRoot);
}

std::unique_ptr<Node> Parser::parseComparison() {// --- think abaut the relativity and eaqulity ( a>b==c and c==b>a case)
	std::unique_ptr<Node> comparisonRoot;

	comparisonRoot = parseExpresion();

	while (checkTokenType(TOKEN_TYPE::EQUAL) || checkTokenType(TOKEN_TYPE::NOT_EQUAL) || checkTokenType(TOKEN_TYPE::LT) 
		|| checkTokenType(TOKEN_TYPE::LT_EQUAL) || checkTokenType(TOKEN_TYPE::GT) || checkTokenType(TOKEN_TYPE::GT_EQUAL)) {

		std::unique_ptr<operatorNode> temp;
		temp = std::make_unique<operatorNode>(advance().type, std::move(comparisonRoot));

		temp->right = parseExpresion();
		comparisonRoot = std::move(temp);
	}

	return std::move(comparisonRoot);
}

std::unique_ptr<Node> Parser::parseExpresion() {
	std::unique_ptr<Node> expresionRoot;

	expresionRoot = parseTerm();

	while (checkTokenType(TOKEN_TYPE::PLUS) || checkTokenType(TOKEN_TYPE::MINUS)) {
		std::unique_ptr<operatorNode> temp;
		temp = std::make_unique<operatorNode>(advance().type, std::move(expresionRoot));

		temp->right = parseTerm();
		expresionRoot = std::move(temp);
	}

	return std::move(expresionRoot);
}

std::unique_ptr<Node> Parser::parseTerm() {
	std::unique_ptr<Node> termRoot;
	
	termRoot = parseFactor();
	
	while (checkTokenType(TOKEN_TYPE::MULT) || checkTokenType(TOKEN_TYPE::DIV)) {
		std::unique_ptr<operatorNode> temp;
		temp = std::make_unique<operatorNode>(advance().type, std::move(termRoot));

		temp->right = parseFactor();
		termRoot = std::move(temp);
	}

	return std::move(termRoot);
}

std::unique_ptr<Node> Parser::parseFactor() {
	if (checkTokenType(TOKEN_TYPE::LPAREN)) {
		advance();//eats '('
		std::unique_ptr<Node> factorRoot = parseExpresion();
		expect(TOKEN_TYPE::RPAREN, "a right paren");//eats ')'
		return factorRoot;
	}
	else if (checkTokenType(TOKEN_TYPE::MINUS)) {
		std::unique_ptr<unaryOperatorNode> temp = std::make_unique<unaryOperatorNode>(advance().type);
		temp->next = parseFactor();
		return temp;
	}
	else if (checkTokenType(TOKEN_TYPE::IDENTIFIER)) {
		return std::make_unique<identifierNode>(advance().value);
	}
	else {
		TOKEN t = expect(TOKEN_TYPE::NUMBER, "a number");
		if (t.value.find(".") != std::string::npos) {
			return std::make_unique<numberNode>(std::stod(t.value));
		}
		else {
			return std::make_unique<numberNode>(std::stoll(t.value));
		}
	}	
}
