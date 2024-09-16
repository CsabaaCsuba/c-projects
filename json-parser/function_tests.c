#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include "rfc_8259_json.h"

int main(){
    JsonValue* FirstVal = init_json_value("miners", STRING);
    printf("type of the first value: %d\nand value: %s\n", FirstVal->type, (char*)FirstVal->ValuePtr);

    int randomnum1 = 10;
    JsonValue* SecondVal = init_json_value(&randomnum1, INT);
    printf("type of the second value: %d\nand value: %d\n", SecondVal->type, *(int*)SecondVal->ValuePtr);

    double randomnum2 = 6.9;
    JsonValue* ThirdVal = init_json_value(&randomnum2, DOUBLE);
    printf("type of the second value: %d\nand value: %.2f\n", ThirdVal->type, *(double*)ThirdVal->ValuePtr);


    int randombool = 1;
    JsonValue* FourthVal = init_json_value(&randombool, BOOL);
    printf("type of the second value: %d\nand value: %.2d\n", FourthVal->type, *(int*)FourthVal->ValuePtr);


    JsonRow* FirstRow = init_row("key1", FirstVal, 1);
    printf("First Row length: %d,\nkey:%s,\nvalue:%s\n", FirstRow->len, FirstRow->KeyPtr, (char*)FirstRow->JsonValuesListPtr->ValuePtr);


    JsonValue* ValueArray = malloc(sizeof(JsonValue)*3);
    ValueArray[0] = *FirstVal;
    ValueArray[1] = *SecondVal;
    ValueArray[2] = *ThirdVal;

    printf("Value of the first array element: %s\n", (char*)ValueArray[0].ValuePtr);
    printf("Value of the second array element: %d\n", *(int*)ValueArray[1].ValuePtr);
    printf("Value of the third array element: %lf\n", *(double*)ValueArray[2].ValuePtr);


    Json* FirstJSON = init_json_from(FirstRow, 1);
    printf("Value of the first json's first row's key: %s\n", (char*)FirstJSON->JsonRowPtr->JsonValuesListPtr->ValuePtr);
    printf("First json's first key: %s\n", FirstJSON->JsonRowPtr[0].KeyPtr);

    

    JsonRow* SecondRow = init_row("boolkey", FourthVal, 1);

    add_row_to_json(FirstJSON, *SecondRow);

    printf("First json's second key: %s\n", FirstJSON->JsonRowPtr[1].KeyPtr);

    printf("Json as string: \n%s\n", stringify_json(FirstJSON, 0));

    //TODO: write more tests for existing functions
    // + write string-to-json converter
}