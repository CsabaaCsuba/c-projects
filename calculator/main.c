#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "ftoa.c"

typedef enum {
    NUMBER,

    OPEN_PAREN,
    CLOSING_PAREN,

    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
} Type;

typedef struct {
    double value;
    Type type;
} Token;

char* token_as_str(Token current_token){
    char temp[50] = {0};
    char* return_ptr = NULL;
    uint8_t len = 0;

    switch (current_token.type){
        case NUMBER:
            ftoa(current_token.value, temp, 2); //todo: implement a better ftoa function
            break;
        case OPEN_PAREN:
            strcpy(temp, "(");
            break;
        case CLOSING_PAREN:
            strcpy(temp, ")");
            break;
        case ADD:
            strcpy(temp, "+");
            break;
        case SUBTRACT:
            strcpy(temp, "-");
            break;
        case MULTIPLY:
            strcpy(temp, "*");
            break;
        case DIVIDE:
            strcpy(temp, "/");
            break;
    }
    printf("temp: %s\n", temp);
    len = strlen(temp)+1;
    return_ptr = malloc(len);
    strncpy(return_ptr, temp, len-1);
    return_ptr[len-1] = '\0';
    return return_ptr;
}



//priority: () -> / * -> + -
void find_first_op(Token* token_arr, uint8_t tokens_len){
    // finds the most inner parenthesis block, 
    // or the first multiplication / division,
    // or the first addition / subtraction
    printf("the last token is %s\n", token_as_str(token_arr[tokens_len]));

}

void tokenize(Token* token_arr, char* input_str) {
    int index = 0;
    while (*input_str) {
        if (*input_str >= '0' && *input_str <= '9') {
            // Parse number
            char* end_ptr;
            double value = strtod(input_str, &end_ptr);
            token_arr[index].type = NUMBER;
            token_arr[index].value = value;
            index++;
            input_str = end_ptr;
        } else {
            // Parse operator or parenthesis
            switch (*input_str) {
                case '+':
                    token_arr[index].type = ADD;
                    break;
                case '-':
                    token_arr[index].type = SUBTRACT;
                    break;
                case '*':
                    token_arr[index].type = MULTIPLY;
                    break;
                case '/':
                    token_arr[index].type = DIVIDE;
                    break;
                case '(':
                    token_arr[index].type = OPEN_PAREN;
                    break;
                case ')':
                    token_arr[index].type = CLOSING_PAREN;
                    break;
                default:
                    // Skip invalid characters
                    input_str++;
                    continue;
            }
            token_arr[index].value = 0; // Non-numeric tokens don't have a value
            index++;
            input_str++;
        }
    }
    // Mark end of tokens
    token_arr[index].type = -1; // -1 to signify end of tokens
    token_arr[index].value = 0;
}



void print_tokens(Token* token_arr, int tokens_len){
    for (int i=0; i<tokens_len; i++){
        printf(" %s ", token_as_str(token_arr[i]));
        if (i % 12 == 0){
            printf("\n");
        }
    }

}


int main(){
    char buf[2048] = {0};

    printf("Input your expression to evaluate: \n");

    scanf("%2047s", buf);

    Token token_arr[100] = {0};

    tokenize(token_arr, buf);

    print_tokens(token_arr, 100);


    return 0;
}