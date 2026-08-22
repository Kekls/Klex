/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/

#include "parser.h"

Parser::Parser(std::vector<TOKEN> tokens) : tokens(std::move(tokens)) {}

std::unique_ptr<Node> Parser::parseExpression() {
	TOKEN token = tokens[current];
	current++;

	return std::make_unique<NumberNode>(std::stof(token.value));
}

void printIndent(int indent)
{
    for (int i = 0; i < indent; i++)
        std::cout << "  ";
}

void printAST(const std::unique_ptr<Node>& node, int indent)
{
    if (!node) return;

    // NUMBER
    if (auto n = dynamic_cast<NumberNode*>(node.get()))
    {
        printIndent(indent);
        std::cout << "Number: " << n->value << "\n";
        return;
    }

    // BINARY
    if (auto b = dynamic_cast<BinaryNode*>(node.get()))
    {
        printIndent(indent);
        std::cout << "BinaryOp: " << (int)b->op << "\n";

        printAST(b->left, indent + 1);
        printAST(b->right, indent + 1);
        return;
    }

    // fallback
    printIndent(indent);
    std::cout << "Unknown node\n";
}