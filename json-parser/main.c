#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "json.h"

int main(){
    int datanum = 1;
    JsonRow* FirstRow = init_row("data", &datanum, INT, 1);
    printf("Json row's value: %d\n", *(int*)(FirstRow->ValuePtr));
    printf("json's key: %s\n", FirstRow->KeyPtr);

    JsonRow* SecondRow = init_row("string", "halohalo", STRING, 1);
    char* s = (char*)SecondRow->ValuePtr;
    printf("Json's second row value: %s\n", s);

    char MyStrArr[2][6] = {"hallo", "hallo"};
    JsonRow* ThirdRow = init_row("stringlist", &MyStrArr, STRING, 2);
    printf("Json's third row values: %s\n", (char*)ThirdRow->ValuePtr+6);


    Json* Json1 = init_json(ThirdRow);
    printf("Json's key count: %d\n", Json1->len);
    printf("Json's value count: %d\n", Json1->JsonRowPtr->len);

    int dataN2 = 3;
    JsonRow* RowX = init_row("data", &dataN2, INT, 1);

    printf("%p\n", add_to_json(Json1, RowX));

    JsonRow* FourthRow = init_row("jsondata", Json1, JSON, 1);
    
    printf("Fourth row's json data: %s\n", (char*)((Json*)(FourthRow->ValuePtr))->JsonRowPtr->ValuePtr);

    Json* Json2 = init_json(FirstRow);
    // printf("Json's json value's first value: %s\n", (char*)((Json*)Json2->JsonRowPtr->ValuePtr)->JsonRowPtr->ValuePtr);

    add_to_json(Json2, FourthRow);

    char* justastring= "I_can't_believe_this";
    JsonRow* FifthRow = init_row("third-key", justastring, STRING, 1);

    add_to_json(Json2, FifthRow);

    // printf("Second json's second keys' value: %d\n", *(int*)Json2->JsonRowPtr[1].ValuePtr);

    // printf("Keys: %s\n", get_keys(Json2)[0]);

    // printf("Json stringified: %s", stringify_json(Json1));

    printf("Second Json stringified: \n%s\n", stringify_json(Json2, 0));

    return 0;
}