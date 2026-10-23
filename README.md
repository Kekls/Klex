# Klex Lang 0.1

## About

Klex Lang is my first real attempt at building a programming language from scratch.

This project is part of my learning journey in compiler design, lexing, parsing, and low-level programming concepts.

At the current stage, only the lexer and mathematical parser are implemented. Future updates will include a full working parser and interpreter.

---

## Features

- Multi-state lexer (DEFAULT / STRING / COMMENT)
- Arithmetic operators support (+, -, *, /, %, etc.)
- String parsing with quotes
- Keyword recognition system
- Token classification (TOKEN_TYPE enum)
- Smart `std::variant` memory nodes for seamless INT/FLOAT evaluation
- Advanced recursive descent math parser (Expression -> Term -> Factor)
- Robust unary minus logic supporting multi-layered chains (e.g., `5 - - - 5`)
- Safe lookahead mechanism with a filtering `peek()` that skips empty rows
- Full transparent multi-line mathematical expression analysis
- Modern C++ memory architecture powered entirely by `std::unique_ptr`
- Integrated AST diagnostic engine with a recursive tree printer

## Limitations

- No interpreter or execution engine
- Minimal error handling

---

## How it works

The lexer reads source code character by character and converts it into a stream of tokens.

Each token represents a meaningful element of the language, such as:

- NUMBER
- STRING
- IDENTIFIER
- OPERATORS
- PARENTHESES
- KEYWORDS

Then based on the tokens provided by lexer parser converts them into AST tree.
---

## Example

### Input:
```txt
5 + -3.14 * (5 + 2 * -8)
```

### Output in debug mode:
```txt
--- LEXER DUMP ---

NUMBER(5)
LINE: 1
COLUMN: 1
PLUS
LINE: 1
COLUMN: 3
MINUS
LINE: 1
COLUMN: 5
NUMBER(3.14)
LINE: 1
COLUMN: 6
MULT
LINE: 1
COLUMN: 11
LPAREN
LINE: 1
COLUMN: 13
NUMBER(5)
LINE: 1
COLUMN: 14
PLUS
LINE: 1
COLUMN: 16
NUMBER(2)
LINE: 1
COLUMN: 18
MULT
LINE: 1
COLUMN: 20
MINUS
LINE: 1
COLUMN: 22
NUMBER(8)
LINE: 1
COLUMN: 23
RPAREN
LINE: 1
COLUMN: 24
END_OF_FILE
LINE: 0
COLUMN: 0

 ---PARSER DUMP---

OPERATOR: PLUS
  OPERATOR: MULT
    OPERATOR: PLUS
      OPERATOR: MULT
        UNARY_OPERATOR: MINUS
          VALUE: 8
        VALUE: 2
      VALUE: 5
    UNARY_OPERATOR: MINUS
      VALUE: 3.14
  VALUE: 5
```
###
---

## Status

This is a development version.

The codebase is not fully optimized yet and will evolve as the language grows (parser + interpreter stages planned).

---

## Roadmap

- [x] Lexer
- [ ] Parser (AST generation)
- [ ] Interpreter
- [ ] Variables & memory system
- [ ] Basic standard library

---

## Author

Kekls  
Contact: bartorgan@gmail.com

