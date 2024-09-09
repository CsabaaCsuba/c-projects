
typedef enum {
    INT,
    DOUBLE,
    STRING,
    JSON,
} TYPE;

typedef struct {
    char* KeyPtr; //only one string value is allowed here.
    void* ValuePtr; //can be a list
    int len; //length of the list
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


    //set type
    CreatedJsonRow->type = InputType;

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
        // printf("Received input value is: %d\n", *(int*)InputValuePtr);
    }
    if (InputType == STRING){
        // char* mystring = (char*)InputValuePtr;
        // printf("Received input string is: %s\n", mystring);
    }


    return CreatedJsonRow;
}

Json* init_json(JsonRow* FirstRowPtr){ //If itfails, returns NULL
    //creates a json struct pointer based on an already existing row's pointer.
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


Json* add_to_json(Json* JsonPtr, JsonRow* RowPtr){
    //the JsonPtr will get reallocated. Everything else stays on the same address.
    if (RowPtr == NULL || JsonPtr == NULL){
        printf("input row ptr or json ptr is null.\n");
        return NULL;
    }
    JsonRow* NewRowPtr = realloc(JsonPtr->JsonRowPtr, (JsonPtr->len+1) * sizeof(JsonRow));
    if (NewRowPtr == NULL){
        printf("new row ptr is null.\n");
        return NULL;
    }
    NewRowPtr[JsonPtr->len] = *RowPtr;

    JsonPtr->JsonRowPtr = NewRowPtr;
    JsonPtr->len = JsonPtr->len + 1;

    return JsonPtr;
}


//functions related to json printing

char* stringify_json(Json* JsonPtr, int JsonLevel);

int do_pointers_exist(Json* JsonPtr){
    if (JsonPtr != NULL){
        if (JsonPtr->JsonRowPtr != NULL){
            return 1;
        }
    }
    return 0;
}

char* extend_with_chars(char* MainStrPtr, char* ExtendChar){
    if (MainStrPtr == NULL || ExtendChar == NULL){
        printf("string cannot be extended, since the main string is null, or the char is not valid\n");
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

char* realloc_string_based_on_value_type(char* MainStrPtr, JsonRow* JsonRowPtr, int JsonLevel){
    int main_str_len = strlen(MainStrPtr);
    char* NewPtr;
    int num_length = 0;
    switch (JsonRowPtr->type){
        case INT:
            int num = *(int*)(JsonRowPtr->ValuePtr);
            printf("number found: %d\n", num);
            
            //TODO: test the ceil thing
            num_length = (int)((ceil(log10(num))+1)*sizeof(char));
            NewPtr = realloc(MainStrPtr, main_str_len+num_length+1);
            if (NewPtr == NULL){
                free(MainStrPtr);
                return NULL;
            }
            MainStrPtr = NewPtr;

            //copy number into the continuation of the string pointer, and place null byte
            sprintf(NewPtr+main_str_len, "%d", num);
            NewPtr[main_str_len+num_length] = '\0';

            // printf("String currently:%s\n", NewPtr);

            
            break;
        case DOUBLE:
            double double_num = *(double*)(JsonRowPtr->ValuePtr);
            printf("non-whole number found: %lf\n", double_num);

            num_length = snprintf(NULL, 0, "%lf", double_num);
            
            NewPtr = realloc(MainStrPtr, main_str_len+num_length+1);
            if (NewPtr == NULL){
                free(MainStrPtr);
                return NULL;
            }
            MainStrPtr = NewPtr;

            //copy number into the continuation of the string pointer, and place null byte
            sprintf(MainStrPtr+main_str_len, "%lf", double_num);
            MainStrPtr[main_str_len+num_length] = '\0';

            // printf("String currently:%s\n", NewPtr);

 
            break;
        case STRING: //TODO: test this
            char* StringFromJson = (char*)JsonRowPtr->ValuePtr;
            printf("String value found: %s\n", StringFromJson);

            int string_length = strlen(StringFromJson);

            NewPtr = realloc(MainStrPtr, main_str_len+string_length+1);
            if (NewPtr == NULL){
                free(MainStrPtr);
                return NULL;
            }
            MainStrPtr = NewPtr;

            //copy string and place null byte.
            strncpy(MainStrPtr+main_str_len, StringFromJson, string_length);
            *(MainStrPtr+main_str_len+string_length) = '\0';

            // printf("String currently:%s\n", NewPtr);
            break;
        case JSON:
            if (do_pointers_exist((Json*)JsonRowPtr->ValuePtr) == 0){ //if there is a valid json pointer with valid json row inside the string
                return NULL;
            }


            char* NestedJsonPtr = (char*)stringify_json((Json*)JsonRowPtr->ValuePtr, JsonLevel);
            if (NestedJsonPtr == NULL){
                free(MainStrPtr);
                return NULL;
            }
            // MainStrPtr = NewPtr; //this line is wrong, it deletes the previous values of the json, and sets it to the inner json only.
            MainStrPtr = extend_with_chars(MainStrPtr, NestedJsonPtr);
            if (MainStrPtr == NULL){
                return NULL;
            }

            break;
        default:
            free(MainStrPtr);
            NewPtr = NULL;
            MainStrPtr = NULL;
            printf("default case reached\n");
            break;
    }

    return MainStrPtr;
}

char* add_key_to_string(char* MainStrPtr, JsonRow* JsonRowPtr){
    printf("function started. String it brought with itself: %s\n", MainStrPtr);
    if (JsonRowPtr == NULL || JsonRowPtr->KeyPtr == NULL){
        printf("pointers are null.\n");
        return NULL;
    }
    int key_length = strlen(JsonRowPtr->KeyPtr);
    int str_length = strlen(MainStrPtr);
    printf("strlen done. \n");
    char* NewPtr = realloc(MainStrPtr, key_length+str_length+1);
    printf("realloc ran\n");
    if (NewPtr == NULL){
        return NULL;
    }

    strncpy(NewPtr+str_length, JsonRowPtr->KeyPtr, key_length);
    NewPtr[str_length+key_length] = '\0'; //add null byte 

    return NewPtr;


}



char* stringify_json(Json* JsonPtr, int JsonLevel){
    char* PaddingPtr = malloc(JsonLevel+1);
    for (int j=0; j<JsonLevel; j++){PaddingPtr[j]='\t';}
    PaddingPtr[JsonLevel] = '\0';
    char* MainStrPtr = malloc(3);
    if (MainStrPtr == NULL){
        printf("initial string malloc failed.\n");
        return NULL;
    }
    *MainStrPtr = '{';
    MainStrPtr[1] = '\n';
    MainStrPtr[2] = '\0';

    if (MainStrPtr == NULL){
        return NULL;
    }

    //if there is no JsonPtr, or if there is no json row, return an empty json.
    if (do_pointers_exist(JsonPtr) == 0){
        char* EndStrPtr = realloc(MainStrPtr, 3);
        if (EndStrPtr != NULL){
            EndStrPtr[1] = '}';
            EndStrPtr[2] = '\0'; 
        }
        return EndStrPtr;
    }

    char* PtrStorage = 0;

    PtrStorage = MainStrPtr;
    printf("ptrstorage right now: %s\n", PtrStorage);

    //add padding based on the level of the json.
    
    

    for (int i=0; i<JsonPtr->len; i++){

        char* StrWithPadding = extend_with_chars(PtrStorage, PaddingPtr);

        char* StrWithKey = add_key_to_string(StrWithPadding, JsonPtr->JsonRowPtr+i); //the entire row pointer is taken
        if (StrWithKey == NULL){
            printf("adding key's string to stringified json failed.\n");
            continue;
        }
        printf("key added to string.\n");

        char* StrWithColon = extend_with_chars(StrWithKey, ":");
        if (StrWithColon == NULL){
            printf("allocating 1 extra byte failed.\n");
            return NULL;
        }

        printf("colon added to string.\n");

        char* StrWithValue = realloc_string_based_on_value_type(StrWithColon, JsonPtr->JsonRowPtr+i, JsonLevel+1);
        if (StrWithValue == NULL){
            printf("String value calculation failed.\n");
            return NULL;
        }

        printf("value added to string: %s\n\n", StrWithValue);

        char* StrWithComma = extend_with_chars(StrWithValue, ",\n");
        if (StrWithComma == NULL){
            printf("allocating 2 extra bytes failed.\n");
            return NULL;
        }
        printf("str with comma: %s\n\n", StrWithComma);

        int len = strlen(StrWithComma);
        printf("len: %d\n", len);
        // free(PtrStorage);
        PtrStorage = malloc(len+1);
        if (PtrStorage == NULL){
            printf("malloc failed for variable PtrStorage.\n");
            return NULL;
        }
        printf("ran\n");
        strncpy(PtrStorage, StrWithComma, len);
        PtrStorage[len] = '\0';

        printf("iteration completed. String now: %s\n", PtrStorage);


        
    }
    
    char* FinalStr;
    FinalStr = extend_with_chars(PtrStorage, PaddingPtr);
    if (FinalStr == NULL){
        return NULL;
    }
    if (JsonLevel > 0){
        FinalStr = extend_with_chars(FinalStr, "}");
        if (FinalStr == NULL){
            return NULL;
        }
    }
        
    else{
        FinalStr = extend_with_chars(PtrStorage, "}\n");
        if (FinalStr == NULL){
            return NULL;
        }
    }
        
    if (FinalStr == NULL){
        printf("allocating 2 extra bytes failed.\n");
        return NULL;
    }

    return FinalStr;

    
}

// char* get_keys(Json* JsonPtr){

//     return "";
// }

// Json* remove_key(Json* JsonPtr, char* KeyPtr){

// }

// void free_json(Json* JsonPtr){

// }