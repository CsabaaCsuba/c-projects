#ifndef Token
    #include "TokenType.hpp"
#endif

// class Lox{
// public:
//     static bool hadError;
//     static void error(int line, char* message);
// };

template <typename T>
class MyVec {
//private:
    
public:
    T* data;
    size_t cap;
    size_t size;
    
    MyVec(size_t cap){
        this->cap = cap;
        this->size = 0;
        this->data = (T*)malloc(sizeof(T)*cap);
        assert(this->data);
    }

    void add(T element){
        if (this->size >= this->cap){
            this->cap++;
            this->data = (T*)realloc((this->data), sizeof(T)*cap);
            assert(this->data);
            this->size++;
            memcpy(this->data+cap-1, &element, sizeof(T));
        }
        else {
            memcpy(this->data+size, &element, sizeof(T));
            this->size++;
        }
    }

    ~MyVec(){ //destructor (called automatically)
        free(this->data);
    }


};

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
        if (type == STRING){
            literalType = String;
        }
        else if (type == NUMBER){
            literalType = Number;
        }
        else {
            literalType = None;
        }

        size_t substring_size = this->current - this->start;
        char* substring = (char*)malloc(substring_size);
        assert(substring);
        memcpy(substring, this->source+start, substring_size);
        substring[substring_size] = '\0';

        switch (literalType){
            case None:
                this->tokens.add(Token(type, substring, line));
                break;
            case Number:
                this->tokens.add(Token(type, substring, *(double*)literalPtr, line));
                break;
            case String:
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
            printf("unterminated string");
            return;
        }

        advance(); //so it jumps over the closing

         //the two " characters don't count, but one has to be there, for the guaranteed string terminator
        size_t substring_size = this->current - this->start - 1;
        char* substring = (char*)malloc(substring_size);
        assert(substring);
        memcpy(substring, this->source+start+1, substring_size);
        substring[substring_size-1] = '\0';

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

        size_t substring_size = this->current - this->start;
        char* substring = (char*)malloc(substring_size + 1);
        assert(substring);
        memcpy(substring, this->source + this->start, substring_size);
        substring[substring_size] = '\0';

        printf("number's substring: %s\n", substring);

        double* parsed_num = (double*)malloc(sizeof(double));
        assert(parsed_num);
        *parsed_num = atof(substring);

        addToken(NUMBER, parsed_num);
    }

    char peekNext(){
        if (current+1 >= source_len) return '\0';
        return source[current+1];
    }

    void identifier(){
        while (isAlphaNumeric(peek())) advance();

        addToken(IDENTIFIER);
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
        assert(this->source);
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