#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct BinaryOp;

typedef union {
    double number;
    struct BinaryOp* expression;
} Expression;

typedef enum {
    EXPR_NUM,
    EXPR_BINARY_OP,

} ExpressionType;

struct BinaryOp{
    struct BinaryOp* LeftPart;
    struct BinaryOp* RightPart;

    bool hasParenthesis;

};

struct AssignOp {
    char identifier_name[128];
    ExpressionType expr_type;
    Expression assigned_expr;
};

int main(){

    //todo: write the parser for this
    char* hardcoded_operation = "x = 3 + 6 / 2";



    return 0;
}