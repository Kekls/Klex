/*
@AUTHOR Kekls
#CONTACT bartorgan@gmail.com
*/
#include "lexer.h"

LEXER::LEXER(std::string path, bool debug) {
    file.open(path, std::ios::binary);

    if (!file) {
        std::cerr << "File '" << path << "' could not be opened!\n";
        std::string err = "file '" + path + "' could not be loaded!\n";
        throw std::runtime_error(err);
    }

    if (debug == true) {
        this->debug = true;
    }
}

void LEXER::start() {
    char stringSep = {};
    std::string buffer = {};

    char peek = {};
    char current = {};

    currentLine = 1;
    currentColumn = 1;

    while (file.get(current)) {
        int next = file.peek();

        if (next != EOF) { peek = static_cast<char>(next); }
        else { peek = {}; }

        if (state == STATE::COMMENT_BLOCK || state == STATE::COMMENT_LINE) {

            commentState(buffer, current, peek, stringSep);
        }
        else if (state == STATE::STRING) {

            stringState(buffer, current, peek, stringSep);
        }
        else if (state == STATE::DEFAULT) {

            defaultState(buffer, current, peek, stringSep);
        }
        else {
            std::cout << "FATAL ERROR textDivision\n";
        }

        currentColumn++;

        if (next == EOF) {
            stateChange(STATE::DEFAULT);
            categorize(buffer);

            stringSep = {};
            TOKEN token = {};

            token.type = TOKEN_TYPE::END_OF_FILE;

            emit(token);
        }
    }

    if (debug) dumpTokens(tokens);
}

void LEXER::commentState(std::string& buffer, char current, char peek, char& stringSep) {
    if (state == STATE::COMMENT_LINE && current == '\n') {
        TOKEN token = {};
        int column = currentColumn - buffer.length();
        emit({ TOKEN_TYPE::COMMENT_LINE, currentLine, column, buffer });

        buffer.clear();

        token = {};
        stateChange(STATE::DEFAULT);
        emit({ TOKEN_TYPE::NEW_LINE, currentLine, currentColumn });
        currentColumn = 0;
        currentLine++;

    }
    else if (state == STATE::COMMENT_BLOCK && current == '*' && peek == '/') {
        TOKEN token = {};

        buffer += std::string(1, current) + std::string(1, peek);
        token.type = TOKEN_TYPE::COMMENT_BLOCK;
        token.value = buffer;
        emit(token);

        stateChange(STATE::DEFAULT);
        buffer.clear();
        file.get();
    }
    else {
        if (current == '\r') {
            return;
        }

        if (state == STATE::COMMENT_BLOCK && current == '\n') {
            currentColumn = 0;
            currentLine++;
        }
        buffer += std::string(1, current);
    }
}

void LEXER::stringState(std::string& buffer, char current, char peek, char& stringSep) {
    if (current == stringSep) {
        TOKEN token = {};

        buffer += std::string(1, current);
        token.type = TOKEN_TYPE::STRING;
        token.value = buffer;
        token.line = currentLine;
        token.column = currentColumn - buffer.length();


        buffer.clear();
        stringSep = {};

        stateChange(STATE::DEFAULT);
        emit(token);
    }
    else {
        buffer += std::string(1, current);
    }
}

void LEXER::defaultState(std::string& buffer, char current, char peek, char& stringSep) {
    if (current == '\n') {
        categorize(buffer);
        emit({ TOKEN_TYPE::NEW_LINE, currentLine, currentColumn });
        currentColumn = 0;
        currentLine++;
        return;
    }
    if (current == '\r') {
        if (peek == '\n') { file.get(); }
        categorize(buffer);
        emit({ TOKEN_TYPE::NEW_LINE, currentLine, currentColumn });
        currentColumn = 0;
        currentLine++;
        return;
    }
    if (isspace(current)) {
        categorize(buffer);
        return;
    }

    switch (current) {

    case '/':
        categorize(buffer);
        if (peek == '*') { stateChange(STATE::COMMENT_BLOCK); buffer = std::string(1, current) + std::string(1, peek); file.get(); currentColumn++; }
        else if (peek == '/') { stateChange(STATE::COMMENT_LINE); buffer = std::string(1, current) + std::string(1, peek); file.get(); currentColumn++; }
        else if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::DIV_ASSIGN, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::DIV, currentLine, currentColumn }); }
        break;

    case '\"':
    case '\'':
        categorize(buffer);
        buffer += std::string(1, current);
        stateChange(STATE::STRING);
        stringSep = current;
        break;

    case '+':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::PLUS_ASSIGN, currentLine, currentColumn - 1 }); }
        else if (peek == '+') { file.get(); currentColumn++; emit({ TOKEN_TYPE::INC, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::PLUS, currentLine, currentColumn }); }
        break;

    case '-':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::MINUS_ASSIGN, currentLine, currentColumn - 1 }); }
        else if (peek == '-') { file.get(); currentColumn++; emit({ TOKEN_TYPE::DEC, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::MINUS, currentLine, currentColumn }); }
        break;

    case '*':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::MULT_ASSIGN, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::MULT, currentLine, currentColumn }); }
        break;

    case '%':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::MOD_ASSIGN, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::MOD, currentLine, currentColumn }); }
        break;

    case '=':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::EQUAL, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::ASSIGN, currentLine, currentColumn }); }
        break;

    case '<':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::LT_EQUAL, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::LT, currentLine, currentColumn }); }
        break;

    case '>':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::GT_EQUAL, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::GT, currentLine, currentColumn }); }
        break;

    case '!':
        categorize(buffer);
        if (peek == '=') { file.get(); currentColumn++; emit({ TOKEN_TYPE::NOT_EQUAL, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::NOT, currentLine, currentColumn }); }
        break;

    case '&':
        categorize(buffer);
        if (peek == '&') { file.get(); currentColumn++; emit({ TOKEN_TYPE::AND, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::BITWISE_AND, currentLine, currentColumn }); }
        break;

    case '|':
        categorize(buffer);
        if (peek == '|') { file.get(); currentColumn++; emit({ TOKEN_TYPE::OR, currentLine, currentColumn - 1 }); }
        else { emit({ TOKEN_TYPE::BITWISE_OR, currentLine, currentColumn }); }
        break;

    case '(': categorize(buffer); emit({ TOKEN_TYPE::LPAREN, currentLine, currentColumn }); break;
    case ')': categorize(buffer); emit({ TOKEN_TYPE::RPAREN, currentLine, currentColumn }); break;
    case '[': categorize(buffer); emit({ TOKEN_TYPE::LBRACK, currentLine, currentColumn }); break;
    case ']': categorize(buffer); emit({ TOKEN_TYPE::RBRACK, currentLine, currentColumn }); break;
    case '{': categorize(buffer); emit({ TOKEN_TYPE::LBRACE, currentLine, currentColumn }); break;
    case '}': categorize(buffer); emit({ TOKEN_TYPE::RBRACE, currentLine, currentColumn }); break;
    case '.': categorize(buffer); emit({ TOKEN_TYPE::DOT, currentLine, currentColumn }); break;
    case ',': categorize(buffer); emit({ TOKEN_TYPE::COMMA, currentLine, currentColumn }); break;
    case ':': categorize(buffer); emit({ TOKEN_TYPE::COLON, currentLine, currentColumn }); break;
    case ';': categorize(buffer); emit({ TOKEN_TYPE::SEMICOLON, currentLine, currentColumn }); break;

    case '\\':
        if (peek == 'n') {
            categorize(buffer);
            file.get();
            currentColumn++;
            emit({ TOKEN_TYPE::NEW_LINE, currentLine, currentColumn });
        }
        break;

    default:
        buffer += std::string(1, current);
        break;
    }
}

void LEXER::stateChange(STATE state) {
    switch (state) {
    case STATE::DEFAULT:
        if(debug) std::cout << "DEFAULT\n";
        this->state = STATE::DEFAULT;
        break;
    case STATE::STRING:
        if (debug) std::cout << "STRING\n";
        this->state = STATE::STRING;
        break;
    case STATE::COMMENT_LINE:
        if (debug) std::cout << "COMMENT_LINE\n";
        this->state = STATE::COMMENT_LINE;
        break;
    case STATE::COMMENT_BLOCK:
        if (debug) std::cout << "COMMENT_BLOCK\n";
        this->state = STATE::COMMENT_BLOCK;
        break;
    default: std::cout << "FATAL ERROR LEXER.stateChange\n";
    }
}

void LEXER::categorize(std::string& buffer) {
    if (buffer.empty()) return;

    TOKEN token = {};

    token.line = currentLine;
    token.column = currentColumn - static_cast<int>(buffer.length());
    token.value = buffer;


    if (KEYWORDS.count(buffer) > 0) {
        token.type = KEYWORDS[buffer];
    }
    else if (isdigit(buffer[0])) {
        token.type = TOKEN_TYPE::NUMBER;
        token.value = buffer;
    }
    else {
        token.type = TOKEN_TYPE::IDENTIFIER;
        token.value = buffer;
    }

    buffer.clear();
    emit(token);
}

void LEXER::emit(TOKEN token) {
    tokens.push_back(token);
}

std::string LEXER::tokenToString(TOKEN_TYPE t) {
    switch (t) {
    case TOKEN_TYPE::NONE: return "NONE";
    case TOKEN_TYPE::NUMBER: return "NUMBER";
    case TOKEN_TYPE::STRING: return "STRING";
    case TOKEN_TYPE::LPAREN: return "LPAREN";
    case TOKEN_TYPE::RPAREN: return "RPAREN";
    case TOKEN_TYPE::PLUS: return "PLUS";
    case TOKEN_TYPE::MINUS: return "MINUS";
    case TOKEN_TYPE::MULT: return "MULT";
    case TOKEN_TYPE::DIV: return "DIV";
    case TOKEN_TYPE::MOD: return "MOD";
    case TOKEN_TYPE::IDENTIFIER: return "IDENTIFIER";
    case TOKEN_TYPE::COMMA: return "COMMA";
    case TOKEN_TYPE::ASSIGN: return "ASSIGN";
    case TOKEN_TYPE::COLON: return "COLON";
    case TOKEN_TYPE::SEMICOLON: return "SEMICOLON";
    case TOKEN_TYPE::LBRACE: return "LBRACE";
    case TOKEN_TYPE::RBRACE: return "RBRACE";
    case TOKEN_TYPE::LBRACK: return "LBRACK";
    case TOKEN_TYPE::RBRACK: return "RBRACK";
    case TOKEN_TYPE::LT: return "LT";
    case TOKEN_TYPE::GT: return "GT";
    case TOKEN_TYPE::DOT: return "DOT";
    case TOKEN_TYPE::NOT: return "NOT";
    case TOKEN_TYPE::COMMENT_LINE: return "COMMENT_LINE";
    case TOKEN_TYPE::COMMENT_BLOCK: return "COMMENT_BLOCK";
    case TOKEN_TYPE::PLUS_ASSIGN: return "PLUS_ASSIGN";
    case TOKEN_TYPE::MINUS_ASSIGN: return "MINUS_ASSIGN";
    case TOKEN_TYPE::MULT_ASSIGN: return "MULT_ASSIGN";
    case TOKEN_TYPE::DIV_ASSIGN: return "DIV_ASSIGN";
    case TOKEN_TYPE::MOD_ASSIGN: return "MOD_ASSIGN";
    case TOKEN_TYPE::EQUAL: return "EQUAL";
    case TOKEN_TYPE::NOT_EQUAL: return "EQUAL";
    case TOKEN_TYPE::LT_EQUAL: return "LT_EQUAL";
    case TOKEN_TYPE::GT_EQUAL: return "GT_EQUAL";
    case TOKEN_TYPE::AND: return "AND";
    case TOKEN_TYPE::OR: return "OR";
    case TOKEN_TYPE::INC: return "INC";
    case TOKEN_TYPE::DEC: return "DEC";
    case TOKEN_TYPE::DO: return "DO";
    case TOKEN_TYPE::TIMES: return "TIMES";
    case TOKEN_TYPE::WHILE: return "WHILE";
    case TOKEN_TYPE::IF: return "IF";
    case TOKEN_TYPE::ELSE: return "ELSE";
    case TOKEN_TYPE::VAR: return "VAR";
    case TOKEN_TYPE::BITWISE_AND: return "BITWISE_AND";
    case TOKEN_TYPE::BITWISE_OR: return "BITWISE_OR";
    case TOKEN_TYPE::NEW_LINE: return "NEW_LINE";
    case TOKEN_TYPE::END_OF_FILE: return "END_OF_FILE";
    default: return "UNKNOWN";
    }
}

std::vector<TOKEN> LEXER::getTokens() {
    return tokens;
}

void LEXER::dumpTokens(std::vector<TOKEN> tokens) {
    for (auto& token : tokens) {
        std::string s;

        s = tokenToString(token.type);
        if (!token.value.empty()) {
            s += "(" + token.value + ")";
        }

        std::cout << s << std::endl;
        std::cout << "LINE: " << token.line << std::endl;
        std::cout << "COLUMN: " << token.column << std::endl;
    }
}
