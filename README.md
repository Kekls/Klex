# Klex Lang 0.1

## About

Klex Lang is my first real attempt at building a programming language from scratch.

This project is part of my learning journey in compiler design, lexing, parsing, and low-level programming concepts.

At the current stage, only the lexer is implemented. Future updates will include a parser and interpreter.

---

## Features

- Multi-state lexer (DEFAULT / STRING / COMMENT)
- Arithmetic operators support (+, -, *, /, %, etc.)
- String parsing with quotes
- Keyword recognition system
- Token classification (TOKEN_TYPE enum)

---

## Limitations

- No parser yet (tokens are not converted into AST)
- No interpreter or execution engine
- Minimal error handling
- Experimental architecture (v0.1)

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

---

## Example

### Input:
```txt
"ala ma kota o imieniu 'jarek' ma on" 5 + 3 * (2 + 1 - 2) "lat"
```

### Output in debug mode:
```txt
STRING
DEFAULT
STRING
DEFAULT
DEFAULT
STRING("ala ma kota o imieniu 'jarek' ma on")
LINE: 1
COLUMN: 0
NUMBER(5)
LINE: 1
COLUMN: 39
PLUS
LINE: 1
COLUMN: 41
NUMBER(3)
LINE: 1
COLUMN: 43
MULT
LINE: 1
COLUMN: 45
LPAREN
LINE: 1
COLUMN: 47
NUMBER(2)
LINE: 1
COLUMN: 48
PLUS
LINE: 1
COLUMN: 50
NUMBER(1)
LINE: 1
COLUMN: 52
MINUS
LINE: 1
COLUMN: 54
NUMBER(2)
LINE: 1
COLUMN: 56
RPAREN
LINE: 1
COLUMN: 57
STRING("lat")
LINE: 1
COLUMN: 58
END_OF_FILE
LINE: 0
COLUMN: 0
```

---

## Status

This is an early experimental version (v0.1).

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

