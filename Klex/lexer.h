/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <fstream>

enum class STATE {//state machine stuff
    DEFAULT,
    STRING,
    COMMENT_LINE,
    COMMENT_BLOCK
};

enum class TOKEN_TYPE {//all types of tokens
    NONE,
    NUMBER,
    STRING,
    LPAREN,
    RPAREN,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    IDENTIFIER,
    COMMA,
    ASSIGN,
    COLON,
    SEMICOLON,
    LBRACE,
    RBRACE,
    LBRACK,
    RBRACK,
    LT,
    GT,
    DOT,
    NOT,
    COMMENT_LINE,
    COMMENT_BLOCK,
    PLUS_ASSIGN,
    MINUS_ASSIGN,
    MULT_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    EQUAL,
    NOT_EQUAL,
    LT_EQUAL,
    GT_EQUAL,
    AND,
    OR,
    INC,
    DEC,
    DO,
    TIMES,
    WHILE,
    IF,
    ELSE,
    VAR,
    BITWISE_AND,
    BITWISE_OR,
    NEW_LINE,
    END_OF_FILE
};

struct TOKEN {//token struct
    TOKEN_TYPE type;
    int line;
    int column;
    std::string value;
};

class LEXER {
private:
    std::vector<TOKEN> tokens;//stores all tokens
    STATE state = STATE::DEFAULT;//current state of lexer
    std::ifstream file;//file
    int currentLine;//points current line of file
    int currentColumn;//points current column of line
    bool debug;//on/off of debug mode

    std::unordered_map<std::string, TOKEN_TYPE> KEYWORDS = {//stores all keywords
        { "do", TOKEN_TYPE::DO},
        { "times", TOKEN_TYPE::TIMES},
        { "while", TOKEN_TYPE::WHILE},
        { "if", TOKEN_TYPE::IF},
        { "else", TOKEN_TYPE::ELSE},
        { "var", TOKEN_TYPE::VAR}
    };

public:
    LEXER(std::string path, bool debug);

    void start();//main function of lexer, sends to other functions based on the state of the machine
    void commentState(std::string& buffer, char current, char peek, char& stringSep);//handles comment state
    void stringState(std::string& buffer, char current, char peek, char& stringSep);//handles string state
    void defaultState(std::string& buffer, char current, char peek, char& stringSep);//handles default state

    void stateChange(STATE state);//handles state change, dumps state changes (debug mode only)

    void categorize(std::string& buffer);//categorizes base tokens NUMBER/INDENTIFIER/KEYWORDS
    void emit(TOKEN token);//pushes token into vector
    std::string tokenToString(TOKEN_TYPE t);//stringifies tokens
    std::vector<TOKEN> getTokens();//returns vector with tokens
    void dumpTokens(std::vector<TOKEN> tokens);//dumps all process info of lexer (debug mode only)

};
