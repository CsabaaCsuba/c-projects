





typedef enum {
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
	END_OF_FILE
} TokenType;

typedef enum {
	Number,
	String,
	None,
} Literal;

class Token {
public:
	TokenType type;
	char* lexeme;
	Literal literal_type;
	void* literal_ptr;
	int line;

	//to handle numbers
	Token(TokenType type, char* input_lexeme, double number, int line) {
		this->type = type;
		printf("input lexeme: %s\n", input_lexeme);
		uint64_t lexeme_length = strlen(input_lexeme)+1;
		this->lexeme = (char*)malloc(lexeme_length);
		if (this->lexeme != nullptr) {
			memcpy(this->lexeme, input_lexeme, lexeme_length);
			this->lexeme[lexeme_length] = '\0';
		}


		this->literal_type = Number;
		this->literal_ptr = malloc(sizeof(double));
		if (this->literal_ptr != nullptr) {
			memcpy(this->literal_ptr, &number, sizeof(double));
		}
		

		this->line = line;
	}

	Token(TokenType type, char* lexeme, char* literal_str, int line) {
		this->type = type;
		uint64_t lexeme_length = strlen(lexeme) + 1;
		this->lexeme = (char*)malloc(lexeme_length);
		if (this->lexeme != nullptr) {
			memcpy(this->lexeme, lexeme, lexeme_length);
			this->lexeme[lexeme_length - 1] = '\0';
		}

		this->literal_type = String;
		uint64_t literal_len = strlen(literal_str) + 1;
		this->literal_ptr = malloc(literal_len);
		if (this->literal_ptr != nullptr) {
			memcpy(this->literal_ptr, literal_str, literal_len);
		}
		

		this->line = line;
	}

	Token(TokenType type, char* lexeme, int line) {
		this->type = type;
		uint64_t lexeme_length = strlen(lexeme) + 1;
		this->lexeme = (char*)malloc(lexeme_length);
		if (this->lexeme != nullptr) {
			memcpy(this->lexeme, lexeme, lexeme_length);
			this->lexeme[lexeme_length - 1] = '\0';
		}

		this->literal_type = None;
		this->literal_ptr = nullptr;

		this->line = line;
	}

	char* toString(){
		char buff[8192] = {0};
		char* return_str = NULL;
		switch (this->literal_type){
			case String:
				sprintf(buff, "Type: %d | Lexeme: %s | Literal: '%s'", this->type, this->lexeme, (char*)literal_ptr);
				break;
			case Number:
				sprintf(buff, "Type: %d | Lexeme: %s | Literal: '%lf'", this->type, this->lexeme, *(double*)literal_ptr);
				break;
			case None:
				sprintf(buff, "Type: %d | Lexeme: %s | Literal: None", this->type, this->lexeme);
				break;
		}
		size_t bufflen = strlen(buff)+1;
		return_str = (char*)malloc(bufflen);
		assert(return_str);
		memcpy(return_str, buff, bufflen);
		return_str[bufflen-1] = '\0';
		
		return return_str;	
	}

	
};