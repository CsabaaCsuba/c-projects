#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printf("Json's third row values: %s", (char*)ThirdRow->ValuePtr+6);




    Json* Json1 = init_json(ThirdRow);
    printf("Json's key count: %d\n", Json1->len);
    printf("Json's value count: %d", Json1->JsonRowPtr->len);

    JsonRow* FourthRow = init_row("jsondata", Json1, JSON, 1);
    
    printf("Fourth row's json data: %s", (char*)((Json*)(FourthRow->ValuePtr))->JsonRowPtr->ValuePtr);

    Json* Json2 = init_json(FourthRow);

    return 0;
}