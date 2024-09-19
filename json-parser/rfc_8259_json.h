
typedef enum {
    INT,
    DOUBLE,
    STRING,
    JSON,
    BOOL,
    // LIST //not implemented yet
} TYPE;

typedef struct {
    void* ValuePtr;
    TYPE type;
} JsonValue;

typedef struct {
    char* KeyPtr; //only one string value is allowed here.
    // void** ValuePtr; //can be a list
    int len; //length of the list
    // TYPE type;
    JsonValue* JsonValuesListPtr; //This can be a list now
} JsonRow;

typedef struct {
    int len;
    JsonRow* JsonRowPtr;
} Json;

// typedef struct {
//     int len;
//     char* ptr;
// }


//constructs a json value pointer, which has a void pointer to any singular value
JsonValue* init_json_value(void* InputValue, TYPE InputType){
    if (InputValue == NULL || InputType > 5){
        printf("input value pointer was null or the input type was incorrect.\n");
        return NULL;
    }
    JsonValue* CreatedJsonVal = malloc(sizeof(JsonValue));
    CreatedJsonVal->type = InputType;

    int single_data_size;

    if (InputType == INT){
        single_data_size = sizeof(int);
    }
    if (InputType == DOUBLE){
        single_data_size = sizeof(double);
    }
    if (InputType == STRING){
        single_data_size = strlen((char*)InputValue)+1;
    }
    if (InputType == JSON){
        single_data_size = sizeof(Json);
    }
    if (InputType == BOOL){
        single_data_size = sizeof(int); //gotta store it as int to reduce complexity
    }

    void* NewMemSpace = malloc(single_data_size);
    if (NewMemSpace == NULL){
        printf("malloc failed\n");
        return NULL;
    }

    memcpy(NewMemSpace, InputValue, single_data_size);
    CreatedJsonVal->ValuePtr = NewMemSpace;

    return CreatedJsonVal;

}

// JsonValue* copy_json_values(JsonValue* SourceValueList, int ValuesLength){
//     JsonValue* NewValuePtr = malloc(sizeof(JsonValue) * ValuesLength);
//     if (NewValuePtr == NULL){
//         printf("malloc failed.\n");
//         return NULL;
//     }
//     for (int i=0; i<ValuesLength; i++){
//         NewValuePtr[i] = 
//     }
// }

//constructs a pointer to an array of pointers. The "ValuesLength" variable specifies the length of the variables.
JsonRow* init_row(char* InputKeyPtr, JsonValue* InputValuesPtr, int ValuesLength){
    // if (ValuesLength < 1){ //InputKeyPtr == NULL || InputValuesPtr == NULL || 
    //     printf("row cannot be initialized. the values length was too short, \
    //     or one of the pointers is null:\n%p\n%p\n%d\n", InputKeyPtr, InputValuesPtr, ValuesLength);

    //     return NULL;
    // }

    JsonRow* CreatedRowPtr = malloc(sizeof(JsonRow));
    if (CreatedRowPtr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    CreatedRowPtr->len = ValuesLength;
    if (InputKeyPtr == NULL){
        CreatedRowPtr->KeyPtr = NULL;
    }
    else {
        int key_length = strlen(InputKeyPtr);
        char* CopiedInputKey = malloc(key_length+1);
        if (CopiedInputKey == NULL){
            printf("malloc failed.\n");
            return NULL;
        }
        strcpy(CopiedInputKey, InputKeyPtr);
        CopiedInputKey[key_length] = '\0';

        CreatedRowPtr->KeyPtr = CopiedInputKey;
    }

    if (InputValuesPtr == NULL){
        CreatedRowPtr->JsonValuesListPtr = NULL;
    }
    else {
        JsonValue* JsonValuesPtr = malloc(ValuesLength*sizeof(JsonValue));
        if (JsonValuesPtr == NULL){
            printf("malloc failed.\n");
            return NULL;
        }
        memcpy(JsonValuesPtr, InputValuesPtr, ValuesLength*sizeof(JsonValue));

        CreatedRowPtr->JsonValuesListPtr = JsonValuesPtr;
    }

    return CreatedRowPtr;
}




//This rfc8259 compliant method copies the JSON RowPtr. 
//Note that it DOES NOT copy the underlying JSON data to new location
Json* init_json_from(JsonRow* RowPtr, int RowsLength){
    //check if all of the pointers are non-null
    int should_break = 1;
    if (RowPtr != NULL){
        if (RowPtr->KeyPtr != NULL  && RowPtr->JsonValuesListPtr != NULL){
            if (RowPtr->JsonValuesListPtr->ValuePtr != NULL){
                should_break = 0;
            }
        }
    }
    if (should_break)
        return NULL;
    
    //create JSON struct and add the values to it
    Json* NewJson = malloc(sizeof(Json));
    if (NewJson == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    NewJson->len = RowsLength;

    //allocate space for all the potential JSON rows
    unsigned int RowsSize = sizeof(JsonRow) * RowsLength;
    JsonRow* NewRowPtr = malloc(RowsSize);
    if (NewRowPtr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    

    //fill up the JSON with the potential array of json rows
    for (int i=0; i<NewJson->len; i++){
        //create enough space for copying the json values;
        NewRowPtr[i].len = RowPtr->len;
        unsigned int JsonValueSize = sizeof(JsonValue) * NewRowPtr[i].len;
        JsonValue* NewValuePtr = malloc(JsonValueSize);
        if (NewValuePtr == NULL){
            printf("malloc failed.\n");
            return NULL;
        }
        memcpy(NewValuePtr, RowPtr->JsonValuesListPtr, JsonValueSize);
        NewRowPtr[i].JsonValuesListPtr = NewValuePtr;

        //copy the key to the memory space inside the row
        unsigned int keysize = strlen(RowPtr[i].KeyPtr);
        NewRowPtr[i].KeyPtr = calloc(keysize+1, 1);
        if (NewRowPtr[i].KeyPtr == NULL){
            printf("malloc failed.\n");
            return NULL;
        }
        strncpy(NewRowPtr[i].KeyPtr, RowPtr[i].KeyPtr, keysize);
    }
    NewJson->JsonRowPtr = NewRowPtr;

    return NewJson;
    
}

Json* add_row_to_json(Json* DestJsonPtr, JsonRow ActualRow){
    if (DestJsonPtr == NULL || ActualRow.JsonValuesListPtr == NULL || ActualRow.KeyPtr == NULL || ActualRow.len == 0){
        printf("one of the values is null / zero: %p\n%p\n%p\n%d\n", DestJsonPtr, ActualRow.JsonValuesListPtr, ActualRow.KeyPtr, ActualRow.len);
        return NULL;
    }
    DestJsonPtr->len++;
    int new_json_row_length = sizeof(JsonRow) * DestJsonPtr->len;
    JsonRow* NewRowsPtr = realloc(DestJsonPtr->JsonRowPtr, new_json_row_length);
    if (NewRowsPtr == NULL){
        printf("realloc failed.\n");
        return NULL;
    }
    memcpy(NewRowsPtr+DestJsonPtr->len-1, &ActualRow, sizeof(JsonRow));

    DestJsonPtr->JsonRowPtr = NewRowsPtr;

    return DestJsonPtr;
}


int check_json_chain(Json* JsonPtr, int row_idx, int val_idx){
    int is_correct = 0;
    if (JsonPtr != NULL){
        if (JsonPtr->JsonRowPtr != NULL){
            if (JsonPtr->JsonRowPtr[row_idx].KeyPtr != NULL){
                if (JsonPtr->JsonRowPtr[row_idx].JsonValuesListPtr != NULL){
                    if (JsonPtr->JsonRowPtr[row_idx].JsonValuesListPtr->ValuePtr != NULL){
                        if ((void*)JsonPtr->JsonRowPtr[row_idx].JsonValuesListPtr[val_idx].ValuePtr != NULL){
                            is_correct = 1;
                        }
                    }
                }
            }
        }
    }

    return is_correct;
}

char* extend_with_chars(char* MainStrPtr, char* ExtendChar){
    if (MainStrPtr == NULL || ExtendChar == NULL){
        printf("string cannot be extended, since the main string is null, or the char array is not valid\n");
        return NULL;
    }
    int string_length = strlen(MainStrPtr);
    int chars_length = strlen(ExtendChar);
    char* NewPtr = realloc(MainStrPtr, string_length+chars_length+1);
    if (NewPtr == NULL){
        return NULL;
    }
    strncpy(NewPtr+string_length, ExtendChar, chars_length);
    NewPtr[string_length+chars_length] = '\0';

    return NewPtr;
}

char* stringify_json(Json* JsonPtr, int JsonLevel);

char* stringify_value(JsonValue* InputValuePtr, int ValuesLength, int JsonLevel){
    if (InputValuePtr == NULL || ValuesLength < 1){
        printf("the input value ptr is incorrect or the length of the values is set below 1.\n");
        return NULL;
    }

    char* Stringified;
    if (ValuesLength > 1){
        Stringified = calloc(2, 1);
        Stringified[0] = '[';
    }
    else {
        Stringified = calloc(1, 1);
    }
    int num_length;
    char* NumStr = NULL;
    for (int i=0; i<ValuesLength; i++){
        switch(InputValuePtr[i].type){
            case INT:
                num_length = (int)((ceil(log10(*(int*)InputValuePtr[i].ValuePtr))+1)*sizeof(char));
                NumStr = calloc(num_length+1, 1);
                if (NumStr == NULL){
                    printf("calloc failed.\n");
                }

                sprintf(NumStr, "%d", *(int*)InputValuePtr[i].ValuePtr);    
                break;
            case DOUBLE:
                num_length = snprintf(NULL, 0, "%lf", *(double*)InputValuePtr[i].ValuePtr);
                NumStr = calloc(num_length+1, 1);
                if (NumStr == NULL){
                    printf("calloc failed.\n");
                }

                sprintf(NumStr, "%lf", *(double*)InputValuePtr[i].ValuePtr);
                break;

            case STRING:
                Stringified = extend_with_chars(Stringified, "\"");
                if (Stringified == NULL){
                    printf("string extension failed.\n");
                    return NULL;
                }
                
                Stringified = extend_with_chars(Stringified, InputValuePtr[i].ValuePtr);
                if (Stringified == NULL){
                    printf("string extension failed.\n");
                    return NULL;
                }

                Stringified = extend_with_chars(Stringified, "\"");
                if (Stringified == NULL){
                    printf("string extension failed.\n");
                    return NULL;
                }

                break;

            case BOOL:
                if (*(int*)InputValuePtr[i].ValuePtr == 0){
                    Stringified = extend_with_chars(Stringified, "false");
                }
                else{
                    Stringified = extend_with_chars(Stringified, "true");
                }
                if (Stringified == NULL){
                    printf("string extension failed.\n");
                    return NULL;
                }
                break;

            case JSON:
                //recursive call to stringify json is necessary
                char* inner_json = stringify_json(InputValuePtr[i].ValuePtr, JsonLevel+1);
                if (inner_json == NULL){
                    printf("stringifying inner json failed.\n");
                    return NULL;
                }
                
                Stringified = extend_with_chars(Stringified, inner_json);
                if (Stringified == NULL){
                    printf("string extension failed.\n");
                    return NULL;
                }
                break;
            default:
                return NULL;
                break;
        }
    }
    if (ValuesLength > 1){
        Stringified = extend_with_chars(Stringified, "]");
        if (Stringified == NULL){
            printf("string extension failed.\n");
            return NULL;
        }
    }

    return Stringified;
}

char* stringify_json(Json* JsonPtr, int JsonLevel){
    char* ReturnStr = calloc(1, 1);
    for (int i=0; i<JsonLevel; i++){
        ReturnStr = extend_with_chars(ReturnStr, "\t");
        if (ReturnStr == NULL){
            printf("String extension failed.\n");
            return NULL;
        }
    }

    printf("string currently: %s(end)", ReturnStr);

    ReturnStr = extend_with_chars(ReturnStr, "{");
    if (ReturnStr == NULL){
        printf("String extension failed.\n");
        return NULL;
    }

    //iterate through each JSON key-value pair
    for (int i=0; i<JsonPtr->len; i++){
        if (check_json_chain(JsonPtr, i, 0) == 0){
            printf("JSON pointer check failed.\n");
            return NULL;
        }
        if (JsonPtr->JsonRowPtr[i].KeyPtr == NULL){
            printf("Key pointer is null.\n");
            return NULL;
        }

        //concat key into the string.
        printf("key detected: %s\n", JsonPtr->JsonRowPtr[i].KeyPtr);

        //add key with string notation.
        ReturnStr = extend_with_chars(ReturnStr, "\"");
        if (ReturnStr == NULL){
            printf("string extension failed.\n");
            return NULL;
        }

        ReturnStr = extend_with_chars(ReturnStr, (char*)JsonPtr->JsonRowPtr[i].KeyPtr);
        if (ReturnStr == NULL){
            printf("String extension failed.\n");
            return NULL;
        }

        ReturnStr = extend_with_chars(ReturnStr, "\"");
        if (ReturnStr == NULL){
            printf("string extension failed.\n");
            return NULL;
        }

        //add the colon which separates key from value.
        ReturnStr = extend_with_chars(ReturnStr, " : ");
        if (ReturnStr == NULL){
            printf("String extension failed.\n");
            return NULL;
        }

        //add the value.

        char* value_string = stringify_value(JsonPtr->JsonRowPtr[i].JsonValuesListPtr, JsonPtr->JsonRowPtr[i].len, JsonLevel+1);
        if (value_string == NULL){
            printf("value string is null.\n");
            return NULL;
        }
        
        ReturnStr = extend_with_chars(ReturnStr, value_string);
        if (ReturnStr == NULL){
            printf("String extension failed.\n");
            return NULL;
        }

        //add a separator comma, and a newline
        if (i == JsonPtr->len-1) //if this is the last iteration
            break;
        ReturnStr = extend_with_chars(ReturnStr, ",\n");
        if (ReturnStr == NULL){
            printf("String extension failed.\n");
            return NULL;
        }

    }
    ReturnStr = extend_with_chars(ReturnStr, "}");
    if (ReturnStr == NULL){
        printf("String extension failed.\n");
        return NULL;
    }
    
    // free(JsonPtr);
    return ReturnStr;
}



