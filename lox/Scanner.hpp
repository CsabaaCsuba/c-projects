#ifndef Token
    #include "TokenType.hpp"
#endif

#ifndef MyVec
    #include "MyVec.hpp"
#endif

// class Lox{
// public:
//     static bool hadError;
//     static void error(int line, char* message);
// };





class Scanner {
private:
    char* source;
    size_t source_len;
    MyVec<Token> tokens = MyVec<Token>(100);
    size_t start = 0;
    size_t current = 0;
    size_t line = 1;
    
    bool isAtEnd(){
        return this->current >= this->source_len;
    }
    
    char advance(){
        current++;
        return source[current-1];
    }

    void addToken(TokenType type, void* literalPtr){
        Literal literalType;
        char* substring = NULL;
        if (type == STRING){
            literalType = String;
            substring = create_substring(this->source, this->start+1, this->current);
        }
        else if (type == NUMBER){
            literalType = Number;
            substring = create_substring(this->source, this->start, this->current);
        }
        else {
            literalType = None;
            substring = create_substring(this->source, this->start, this->current+1);
        }


        switch (literalType){
            case None:
                this->tokens.add(Token(type, substring, line));
                break;
            case Number:
                if (literalPtr == NULL){
                    printf("the literal's pointer was null. (Parsed a Number)\n");
                }
                this->tokens.add(Token(type, substring, *(double*)literalPtr, line));
                break;
            case String:
                if (literalPtr == NULL){
                    printf("the literal's pointer was null. (Parsed a String)\n");
                }
                this->tokens.add(Token(type, substring, (char*)literalPtr, line));
                break;
        }
    }

    void addToken(TokenType type){
        addToken(type, nullptr);
    }

    void scanToken(){
        
        char c = advance();
        printf("current char: %c\n", c);
        switch (c){
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_BRACE); break;
            case '}': addToken(RIGHT_BRACE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case '-': addToken(MINUS); break;
            case '+': addToken(PLUS); break;
            case ';': addToken(SEMICOLON); break;
            case '*': addToken(STAR); break;

            case '!':
                addToken(match('=') ? BANG_EQUAL : BANG);
                break;
            case '=':
                addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                break;
            case '<':
                addToken(match('=') ? LESS_EQUAL : LESS);
                break;
            case '>':
                addToken(match('=') ? GREATER_EQUAL : GREATER);
                break;
            case '/':
                if (match('/')) {
                    while (peek() != '\n' && !isAtEnd()) advance();
                }
                else {
                    addToken(SLASH);
                }
                break;

            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;

            case '"':
                string();
                break;

            case 'o':
                if (peek() == 'r' && !isAlphaNumeric(peekNext())) {
                    advance(); // Consume 'r'
                    addToken(OR);
                }
                break;
            default:
                if (isDigit(c)){
                    number();
                }
                else if (isAlpha(c)){
                    identifier();
                }
                else {
                    // Lox::error(line, (char*)"Unexpected character");
                    printf("Unexpected character: %c (ascii)\n", c);
                }
                
                break;
        }
    }

    bool match(char expected){
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;
        current++;
        return true;
    }

    char peek() {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    void string(){
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') this->line++;
            advance();
        }

        if (isAtEnd()){
            // Lox::error(line, (char*)"Unterminated string.");
            printf("unterminated string\n");
            return;
        }

        advance(); //so it jumps over the closing

         //the two " characters don't count, but one has to be there, for the guaranteed string terminator

        char* substring = create_substring(this->source, this->start+1, this->current);

        printf("found string: %s\n", substring);

        addToken(STRING, (void*)substring);
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    } 
    void number(){
        while(isDigit(peek())) advance();

        if (peek() == '.' && isDigit(peekNext())){
            advance();
            while (isDigit(peek())) advance();
        }

        char* substring = create_substring(this->source, this->start, this->current+1);

        printf("number's substring: %s\n", substring);

        double* parsed_num = (double*)malloc(sizeof(double));
        assert_ptr(parsed_num);
        *parsed_num = atof(substring);

        addToken(NUMBER, parsed_num);
    }

    char peekNext(){
        if (current+1 >= source_len) return '\0';
        return source[current+1];
    }

    void identifier(){

        TokenType type = IDENTIFIER;
        while (isAlphaNumeric(peek())) advance();

        char* substring = create_substring(this->source, this->start, this->current+1);

        if (strcmp(substring, "and") == 0){
            type = AND;
        }
        else if (strcmp(substring, "class") == 0){
            type = CLASS;
        }
        else if (strcmp(substring, "else") == 0){
            type = ELSE;
        }
        else if (strcmp(substring, "false") == 0){
            type = FALSE;
        }
        else if (strcmp(substring, "for") == 0){
            type = FOR;
        }
        else if (strcmp(substring, "fun") == 0){
            type = FUN;
        }
        else if (strcmp(substring, "if") == 0){
            type = IF;
        }
        else if (strcmp(substring, "nil") == 0){
            type = NIL;
        }
        else if (strcmp(substring, "or") == 0){
            type = OR;
        }
        else if (strcmp(substring, "print") == 0){
            type = PRINT;
        }
        else if (strcmp(substring, "return") == 0){
            type = RETURN;
        }
        else if (strcmp(substring, "super") == 0){
            type = SUPER;
        }
        else if (strcmp(substring, "this") == 0){
            type = THIS;
        }
        else if (strcmp(substring, "true") == 0){
            type = TRUE;
        }
        else if (strcmp(substring, "var") == 0){
            type = VAR;
        }
        else if (strcmp(substring, "while") == 0){
            type = WHILE;
        }


        free(substring);

        addToken(type);
    }

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }
public:
    Scanner(char* source){
        // this->source = source;
        this->source_len = strlen(source)+1;
        this->source = (char*)malloc(this->source_len);
        assert_ptr(this->source);
        memcpy(this->source, source, this->source_len);

    }
    MyVec<Token> scanTokens(){
        while (!isAtEnd()){
            this->start = this->current;
            scanToken();
        }
        this->tokens.add(Token(END_OF_FILE, (char*)"", this->line));
        printf("scan ended.\n");
        return this->tokens;
    }


};