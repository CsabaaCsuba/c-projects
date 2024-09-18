
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
    if (InputKeyPtr == NULL || InputValuesPtr == NULL || ValuesLength < 1){
        printf("row cannot be initialized. the values length was too short, \
        or one of the pointers is null:\n%p\n%p\n%d\n", InputKeyPtr, InputValuesPtr, ValuesLength);

        return NULL;
    }

    JsonRow* CreatedRowPtr = malloc(sizeof(JsonRow));
    if (CreatedRowPtr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    CreatedRowPtr->len = ValuesLength;

    int key_length = strlen(InputKeyPtr);
    char* CopiedInputKey = malloc(key_length+1);
    if (CopiedInputKey == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    strcpy(CopiedInputKey, InputKeyPtr);
    CopiedInputKey[key_length] = '\0';

    CreatedRowPtr->KeyPtr = CopiedInputKey;


    JsonValue* JsonValuesPtr = malloc(ValuesLength*sizeof(JsonValue));
    if (JsonValuesPtr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    memcpy(JsonValuesPtr, InputValuesPtr, ValuesLength*sizeof(JsonValue));

    CreatedRowPtr->JsonValuesListPtr = JsonValuesPtr;


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


char* remove_character(char* original, char char_to_remove){
    if (original == NULL || char_to_remove == 0){
        return NULL;
    }
    int original_length = strlen(original);
    if (original_length == 0) return NULL;

    char* NewArr = malloc(original_length+1);
    if (NewArr == NULL) return NULL;

    int new_strlen = 0;
    for (int i=0; i<original_length; i++){
        if (original[i] == char_to_remove){
            
        }
        else {
            NewArr[new_strlen]  = original[i];
            new_strlen++;
        }
    }
    NewArr[new_strlen] = '\0';
    NewArr = realloc(NewArr, new_strlen+1);
    // original = realloc(original, new_strlen+1);
    // strcpy(original, NewArr);
    // original[new_strlen] = '\0';
    if (NewArr == NULL) return NULL;

    return NewArr;

}


//parses the first quote separated part of the string
char* parse_first_str_slice(char* StringPartPtr){
    if (*StringPartPtr == '\"'){
        char* LastQuotePosPtr = strchr(StringPartPtr+1, '\"');
        if (LastQuotePosPtr == NULL){
            printf("the string wasn't terminated properly.\n");
            return NULL;
        }
        int index = LastQuotePosPtr - StringPartPtr;
        printf("the end of the string is at index %d, after character (%c)\n", index, *(StringPartPtr+index-1));

        char* ParsedStr = malloc(index);
        if (ParsedStr == NULL){
            printf("malloc failed.\n"); 
            return NULL;
        }
        ParsedStr[index-1] = '\0';

        strncpy(ParsedStr, StringPartPtr+1, index-1);
        printf("parsed string in the end: %s\n", ParsedStr);
        
        return ParsedStr;
    }
    else return NULL;
}

JsonValue* parse_list(char* CurrentStringIdx){

}

//parses any type of value from a JSON-formatted char array
JsonValue* parse_value(char* StringPartPtr){
    JsonValue* NewJsonValue = malloc(sizeof(JsonValue));
    if (NewJsonValue == NULL){
        printf("malloc failed.\n");
        return NULL;
    }

    int string_len = strlen(StringPartPtr);
    for (int i=0; i<string_len; i++){

    }


}

Json* tokenize_json(char* RawJsonString, int JsonStringLength){
    if (RawJsonString == NULL || JsonStringLength < 1){
        printf("json string invalid\n");
        return NULL;
    }

    //removes any formatting characters from the json
    char format_chars[] = "\t\n\v\f\r ";
    char* WorkString = RawJsonString;
    for (long unsigned int i=0; i<strlen(format_chars); i++){
        WorkString = remove_character(WorkString, format_chars[i]);
    }
    JsonStringLength = strlen(WorkString);
    
    //allocate enough space for one JSON to be stored
    Json* NewJsonPtr = malloc(sizeof(Json));
    if (NewJsonPtr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }
    // int current_json_size = 1;
    
    
    if (WorkString[0] == '{' && WorkString[JsonStringLength-1] == '}'){
        int is_key_imminent = 1;
        printf("this is a singular JSON.\n");
        //for loop to traverse through the JSON, and fill up the JSON object with the correct values.
        for (int i=1; WorkString[i] != '}'; i++){ //Because Other JSON Objects will be parsed recursively, this character can denote the ending of a JSON
            printf("char: %c\n", WorkString[i]);
            if (is_key_imminent == 1){
                if (WorkString[i] == '\"'){
                    char* KeyPtr = parse_first_str_slice(WorkString+i);
                    if (KeyPtr == NULL) return NULL;
                    printf("received key: %s\n", KeyPtr);
                    i+=strlen(KeyPtr)+1; //make i jump forward, so the closing quotation mark is skipped too
                    

                    if (WorkString[i+1] == ':'){
                        i++;
                        is_key_imminent = 0;
                    }
                    else {
                        printf("colon expected at index %d\n", i);
                    }
                }
                else {
                    printf("awaited double quotes for key, got something else.\n");
                    return NULL;
                }
            }
            else {
                JsonValue* CreatedJsonValue = parse_value(WorkString+i);


                if (WorkString[i+1] == ','){
                        i++;
                        is_key_imminent = 1;
                }
                else if (WorkString[i+1] == '}'){

                }
                else {
                    printf("String formatting problems at index %d\n", i+1);
                    return NULL;
                }
            }
        }
    }
    else if (WorkString[0] == '[' && WorkString[JsonStringLength-1] == ']'){
        // JsonValue* JSONListPtr = parse_list(WorkString);
        printf("this will probably be an array of multiple JSONs.\n");
    }
    else {
        printf("Raw JSON string invalid.\n");
        return NULL;
    }

    


    
    return NewJsonPtr;
}

// // char* get_keys(Json* JsonPtr){

// //     return "";
// // }

// // Json* remove_key(Json* JsonPtr, char* KeyPtr){

// // }

// // void free_json(Json* JsonPtr){

// // }