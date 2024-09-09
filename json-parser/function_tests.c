#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "json.h"

int main(){
    int datanum = 2;
    JsonRow* FirstRow = init_row("data", &datanum, INT, 1);
    printf("Json row's value: %d\n", *(int*)(FirstRow->ValuePtr));
    printf("json's key: %s\n", FirstRow->KeyPtr);

    
    char* bruh = malloc(2);
    bruh[0] = '{';
    bruh[1] = '\0';
    realloc_string_based_on_value_type(bruh, FirstRow);
    printf("ran\n");

    double float_data = 44.8797;
    JsonRow* SecondRow = init_row("float-data", &float_data, DOUBLE, 1);
    realloc_string_based_on_value_type(bruh, SecondRow);


    char* my_char_arr = "halohalo";
    JsonRow* ThirdRow = init_row("string-data", my_char_arr, STRING, 1);
    char* newdata = malloc(2);
    newdata[0] = 'o';
    newdata[1] = '\0';
    printf("%s\n", add_key_to_string(my_char_arr, ThirdRow));


}