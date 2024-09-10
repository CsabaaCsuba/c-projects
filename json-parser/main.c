#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "json.h"
// #include "parser.h"

int main(){
    int* datanum = malloc(sizeof(int));
    *datanum = 99;
    int** DataPtr = malloc(sizeof(int*));
    DataPtr[0] = datanum;

    JsonRow* FirstRow = init_row("data", (void**)DataPtr, INT, 1);
    printf("Json row's value: %d\n", *(int*)(FirstRow->ValuePtr));
    printf("json's key: %s\n", FirstRow->KeyPtr);
    

    char** StringPtr = malloc(sizeof(char*));
    char* MyString = calloc(9, 1);
    strcpy(MyString, "halohalo");
    StringPtr[0] = MyString;
    JsonRow* SecondRow = init_row("string", (void**)StringPtr, STRING, 1);
    char* s = (char*)SecondRow->ValuePtr[0];
    printf("Json's second row value: %s\n", s);



    char** MyStrArr = malloc(sizeof(char*)*2);
    char* FirstElem = calloc(7, 1);
    strcpy(FirstElem, "hallo");
    char* SecondElem = calloc(8, 1);
    strcpy(SecondElem, "digger");

    MyStrArr[0] = FirstElem;
    MyStrArr[1] = SecondElem;

    JsonRow* ThirdRow = init_row("stringlist", (void**)MyStrArr, STRING, 2);
    printf("Json's third row values: %s\n", (char*)ThirdRow->ValuePtr+6);


    Json* Json1 = init_json(SecondRow);
    printf("Json's key count: %d\n", Json1->len);
    printf("Json's value count: %d\n", Json1->JsonRowPtr->len);

    int** dataN2Storer = malloc(sizeof(int*)*2);
    int* dataN2 = malloc(sizeof(int));
    int* dataN3 = malloc(sizeof(int));
    *dataN2 = 2;
    *dataN3 = 66;
    dataN2Storer[0] = dataN2;
    dataN2Storer[1] = dataN3;
    JsonRow* RowX = init_row("data", (void**)dataN2Storer, INT, 2);

    

    printf("%p\n", add_to_json(Json1, RowX));

    Json** JsonStorer = malloc(sizeof(Json*));
    JsonStorer[0] = Json1;

    JsonRow* FourthRow = init_row("jsondata", (void**)JsonStorer, JSON, 1);
    
    printf("Fourth row's json data: %s\n", (char*)((Json*)(FourthRow->ValuePtr[0]))->JsonRowPtr->ValuePtr[0]);



    Json* Json2 = init_json(FirstRow);
    // printf("Json's json value's first value: %s\n", (char*)((Json*)Json2->JsonRowPtr->ValuePtr)->JsonRowPtr->ValuePtr);

    add_to_json(Json2, FourthRow);
    add_to_json(Json2, ThirdRow);

    char** JustAStorage = malloc(sizeof(char*));
    char* justastring= "I_can't_believe_this";
    JustAStorage[0] = justastring;
    JsonRow* FifthRow = init_row("third-key", (void**)JustAStorage, STRING, 1);

    add_to_json(Json2, FifthRow);

    // printf("Second json's second keys' value: %d\n", *(int*)Json2->JsonRowPtr[1].ValuePtr);

    // printf("Keys: %s\n", get_keys(Json2)[0]);

    // printf("Json stringified: %s", stringify_json(Json1));

    printf("Second Json stringified: \n%s\n", stringify_json(Json2, 0));

    Json** two_jsons = malloc(sizeof(Json*)*2);
    two_jsons[0] = Json1;
    two_jsons[1] = Json2;
    JsonRow* SixthRow = init_row("json-list", (void**)two_jsons, JSON, 2);

    Json* Json3 = init_json(SixthRow);

    printf("the two jsons in one: %s", stringify_json(Json3, 0));


    int** boolstorage = (int**)malloc(sizeof(int*));
    int* boolvalue = malloc(sizeof(int));
    *boolvalue = 5;
    boolstorage[0] = boolvalue;
    JsonRow* SeventhRow = init_row("bool-data", (void**)boolstorage, BOOL, 1);

    Json* Json4 = init_json(SeventhRow);

    printf("bool parse testing: \n%s\n", stringify_json(Json4, 0));

    // printf("First Json stringified: \n%s\n", stringify_json(Json1, 0));

    return 0;
}