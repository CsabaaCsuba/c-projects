typedef enum {
    INT,
    DOUBLE,
    STRING,
    JSON,
} TYPE;

typedef struct {
    char* KeyPtr; //only one string value is allowed here.
    void* ValuePtr; //can be a list
    int len;
    TYPE type;
} JsonRow;

typedef struct {
    int len;
    JsonRow* JsonRowPtr;
} Json;

JsonRow* init_row(char* InputKeyPtr, void* InputValuePtr, TYPE InputType, int ValuesLength){
    if (InputKeyPtr == NULL || InputValuePtr == NULL || InputType > 3){
        printf("Null pointers received or InputType is incorrect value.\n");
        return NULL;
    }

    //initializing the json row
    JsonRow* CreatedJsonRow = malloc(sizeof(JsonRow));
    if (CreatedJsonRow == NULL){
        printf("Creating Json Row failed...\n");
        return NULL;
    }

    //copying the InputKeyPtr string to an other location.
    
    CreatedJsonRow->KeyPtr = malloc(strlen(InputKeyPtr)+1);
    if (CreatedJsonRow->KeyPtr == NULL){
        printf("Creating json key ptr failed...\n");
        return NULL;
    }
    if (memcpy(CreatedJsonRow->KeyPtr, InputKeyPtr, strlen(InputKeyPtr)+1) == NULL){
        printf("Copying key string to an other location failed.");
        return NULL;
    }


    //allocating space for the input values
    int InputTypesSize;
    if (InputType == INT){
        InputTypesSize = 4;
    }
    else if (InputType == DOUBLE){
        InputTypesSize = 8; //a double takes up 8 bytes
    }
    else if (InputType == STRING){
        InputTypesSize = strlen(InputValuePtr)+1; //account for null-byte
        /*because valueslength should be the actual number of separate strings,
        the inputtypesize should be the length of one individual string
        */
    }
    else {
        InputTypesSize = sizeof(Json);
    }

    CreatedJsonRow->len = ValuesLength;
    CreatedJsonRow->ValuePtr = malloc(InputTypesSize * ValuesLength);
    if (CreatedJsonRow->ValuePtr == NULL){
        printf("malloc for values' ptr failed.\n");
        return NULL;
    }
    if (memcpy(CreatedJsonRow->ValuePtr, InputValuePtr, ValuesLength*InputTypesSize) == NULL){
        printf("copying values into json row failed.\n");
        return NULL;
    }

    

    if (InputType == INT){
        printf("Received input value is: %d\n", *(int*)InputValuePtr);
    }
    if (InputType == STRING){
        char* mystring = (char*)InputValuePtr;
        printf("Received input string is. %s\n", mystring);
    }


    return CreatedJsonRow;
}

Json* init_json(JsonRow* FirstRowPtr){ //If fails, returns NULL
    if (FirstRowPtr == NULL){
        printf("There should be an actual first row.\n");
        return NULL;
    }
    Json* CreatedJsonPtr = malloc(sizeof(Json));
    if (CreatedJsonPtr == NULL){
        printf("malloc for json ptr failed.\n");
        return NULL;
    }

    CreatedJsonPtr->len = 1; //one for the first value
    CreatedJsonPtr->JsonRowPtr = FirstRowPtr;

    return CreatedJsonPtr;
}


//untested
Json* add_to_json(Json* JsonPtr, JsonRow* RowPtr){
    if (RowPtr == NULL || JsonPtr == NULL){
        printf("input row ptr or json ptr is null.\n");
        return NULL;
    }
    JsonRow* NewRowPtr = realloc(JsonPtr->JsonRowPtr, JsonPtr->len+sizeof(JsonRow)+1);
    if (NewRowPtr == NULL){
        printf("new row ptr is null.\n");
        return NULL;
    }
    NewRowPtr[JsonPtr->len] = *RowPtr;

    JsonPtr->JsonRowPtr = NewRowPtr;

    return JsonPtr;
}

// char* get_keys(Json* JsonPtr){

//     return "";
// }