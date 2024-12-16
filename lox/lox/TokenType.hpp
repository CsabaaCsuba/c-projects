//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

enum TokenType {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,
	// Literals.
	IDENTIFIER, STRING, NUMBER,
	// Keywords.
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
	EOF
};

enum Literal {
	Number,
	String,
	None,
};

class Token {
public:
	TokenType type;
	char* lexeme;
	Literal literal_type;
	void* literal_ptr;
	int line;

	Token(TokenType type, char* lexeme, double number, int line) {
		this->type = type;
		uint64_t lexeme_length = strlen(lexeme)+1;
		this->lexeme = (char*)malloc(lexeme_length);
		strncpy(this->lexeme, lexeme, lexeme_length - 1);
		this->lexeme[lexeme_length - 1] = '\0';

		this->literal_type = Number;
		this->literal_ptr = malloc(sizeof(double));
		memcpy(this->literal_ptr, &number, sizeof(double));

		this->line = line;
	}

	Token(TokenType type, char* lexeme, char* literal_str, int line) {
		this->type = type;
		uint64_t lexeme_length = strlen(lexeme) + 1;
		this->lexeme = (char*)malloc(lexeme_length);
		strncpy(this->lexeme, lexeme, lexeme_length - 1);
		this->lexeme[lexeme_length - 1] = '\0';

		this->literal_type = String;
		uint64_t literal_len = strlen(literal_str) + 1;
		this->literal_ptr = malloc(literal_len);
		memcpy(this->literal_ptr, literal_str, literal_len);

		this->line = line;
	}

	Token(TokenType type, char* lexeme, double number, int line) {
		this->type = type;
		uint64_t lexeme_length = strlen(lexeme) + 1;
		this->lexeme = (char*)malloc(lexeme_length);
		strncpy(this->lexeme, lexeme, lexeme_length - 1);
		this->lexeme[lexeme_length - 1] = '\0';

		this->literal_type = Number;
		this->literal_ptr = malloc(sizeof(double));
		memcpy(this->literal_ptr, &number, sizeof(double));

		this->line = line;
	}

	
};