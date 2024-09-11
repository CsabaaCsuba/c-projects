#ifndef Json
    #include "json.h"
#endif
#include <stdlib.h>
#include <stdio.h>

/*
JSON rules:
    1. always starts with "{" and ends with "}"
    2. keys always look like this: "key1"
    3. values can look like these:
        9, "value", [0, 1, 2], {"is_null":true}, [0, "asd", {"data":{"key":"value"}}]
    
    4. data types: int, string, list, json
    5. an other json format can be anywhere in the file.


EXTRA:
    - the main JSON can be list as well, containing multiple smaller jsons, or primitive data
    - if the main JSON is not a JSON, it will have the key "a"

*/

// Json** parse_json_list(char* RawJsonPtr){

// }

// Json* parse_json(char* RawJsonPtr){

// }

enum TOKEN_TYPE {
    INT,
    DOUBLE,
    STRING,
    JSON,
    BOOL,
    LIST
}

void** read_list(char* RawStringPtr, Json* CurrentJsonPtr){

}

Json** read_raw_string(char* RawJsonPtr, char delimiter){ //returns an array of JSON pointers
    int is_in_list = 0;
    int is_value_next = 0;
    int RawStringLen = strlen(RawJsonPtr);
    Json** JsonFormat = (Json**)calloc(sizeof(Json*), 1);

    
    
    for (int i=0; RawJsonPtr[i] != delimiter; i++){
        if (is_value_next == 1){
                
        }
        if (RawJsonPtr[i] == '['){
            printf("The whole thing is a list.\n");
            is_in_list = 1;
        }
        else if (RawJsonPtr[i] == '{'){
            printf("The whole thing is inside a json.\n");
        }
        else if (RawJsonPtr[i] == '"'){
            
            printf("String is imminent.\n");
        }
        else if (RawJsonPtr[i] == ' ' || RawJsonPtr[i] == '\n' || RawJsonPtr[i] == "\r"){
            continue;
        }
        else {
            printf("this thing cannot even be parsed.\n");
        }
    }

    return JsonFormat;
}

// Json* read_response(Json* JsonPtr){

// }