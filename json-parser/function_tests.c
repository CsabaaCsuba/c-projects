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

    printf("Value of the second array element: %d\n", *(int*)ValueArray[1].ValuePtr);

}