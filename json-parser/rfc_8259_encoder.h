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

//doesn't require error handling
void shift_bytes_down(char* str, int amount_to_shift) {
    int len = strlen(str);
    if (amount_to_shift >= len) {
        return;
    }
    memmove(str, str + amount_to_shift, len - amount_to_shift + 1); // +1 for null terminator
}

//parses the first quote separated part of the string
char* parse_quoted_string(char* StringPartPtr){
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




/*

    Raw String -> remove formatting chars -> Main function (infer the type of the main json)
        -> if non-json, throw it into a value parser function
            -> If it's a list, call a list-parser function 
                -> (which can have recursive calls to value parser and json parser, and even the list-parser)
        -> if json, throw it into a json parser function
            -> it checks for the stringified key, then for the ":"" character, and the ","

    RULE: every function only parses until the first legit value of it's type. Singular values should be parsed until the next character is"," or "}"
    RULE: a JSON is now just considered an object
    
    RULE: if the FIRST JsonRow's KeyPtr is empty, the entire JSON is considered primitive, and it cannot have other JsonRows

*/
int increment_val = 0;

//this function shifts the bytes as well.
JsonValue* parse_primitive_value(char* CurrentStrPtr){


}

Json* parse_value(char* CurrentStrPtr){
    if (CurrentStrPtr == NULL){
        printf("String is NULL\n");
        return NULL;
    }
    Json* ReturnJsonPtr = malloc(sizeof(Json));
    if (ReturnJsonPtr == NULL){
        printf("JSON parsing failed.\n");
        return NULL;
    }

    if (CurrentStrPtr[0] == '{'){ //the main Object is a JSON, and can have multiple rows

    }
    else if (CurrentStrPtr[0] == '['){ //the main Object is a list, and can only have one JsonRow, but that can have any type of multiple Json values
        //expand the same JSON values.
        shift_bytes_down(CurrentStrPtr, 1);
        for (int i=0; CurrentStrPtr[i] != ']'; i++){
            Json* LocalJson = parse_value(CurrentStrPtr);
        }
        if (CurrentStrPtr[0] == ']'){
            shift_bytes_down(CurrentStrPtr, 1);
        }

    }
    else if (CurrentStrPtr[0] == '\"'){ //the main Object is a string, and can only have one JsonRow
        char* parsed_value = parse_quoted_string(CurrentStrPtr);
        if (parsed_value == NULL){
            printf("parsing string from double quotes failed.\n");
            return NULL;
        }
        printf("parsed value: %s\n", parsed_value);
        shift_bytes_down(CurrentStrPtr, (parsed_value)+2); //+2 for the double quotes
        printf("Raw JSON string after parsing a string out: %s\n", CurrentStrPtr);

        return NULL; //???
    }
    else if (CurrentStrPtr[0] >= 48 && CurrentStrPtr[0] <= 57){ //if the current string part points to a number
        //TODO: pack these into the function as well.
        char* NumEndIdx;
        double* ParsedNumPtr = malloc(sizeof(double));
        *ParsedNumPtr = strtod(CurrentStrPtr, &NumEndIdx);
        printf("parsed number: %lf", *ParsedNumPtr);
        int num_len = NumEndIdx - CurrentStrPtr;
        shift_bytes_down(CurrentStrPtr, NumEndIdx);
        JsonValue* JsonVal = init_json_value(ParsedNumPtr, DOUBLE);
        if (JsonVal == NULL){return NULL;}
        ReturnJsonPtr->JsonRowPtr = init_row(NULL, JsonVal, 1);
        if (ReturnJsonPtr->JsonRowPtr == NULL){return NULL;}
        ReturnJsonPtr->JsonRowPtr->JsonValuesListPtr = JsonVal;
        ReturnJsonPtr->JsonRowPtr->KeyPtr = NULL;
        ReturnJsonPtr->JsonRowPtr->len = 1;
        ReturnJsonPtr->len = 1;



    }
    //handle boolean value
    else if (CurrentStrPtr[0] == 't' || CurrentStrPtr == 'f'){
            //TODO: pack these into the function as well.
            int* BoolStorage = malloc(sizeof(int));
            
            JsonValue* JsonVal = init_json_value(BoolStorage, BOOL);
            JsonRow* OnlyRow = init_row(NULL, JsonVal, 1);
            ReturnJsonPtr->JsonRowPtr = OnlyRow;
            ReturnJsonPtr->len = 1;
        if (strncmp(CurrentStrPtr, "true", 4) == 1){
            *(BoolStorage) = 1;
            shift_bytes_down(CurrentStrPtr, 4);
        }
        else if (strncmp(CurrentStrPtr, "false", 5) == 1){
            *(BoolStorage) = 0;
            shift_bytes_down(CurrentStrPtr, 5);
        }
        else {
            printf("boolean value wasn't true, neither false. returning...\n");
            free(BoolStorage);
            free(JsonVal);
            free(OnlyRow);
            free(ReturnJsonPtr);
            return NULL;
        }
    }

    return ReturnJsonPtr;
}

Json* start_json_parsing(char* OriginalStr){
    if (OriginalStr == NULL){
        printf("input string was null.\n");
        return NULL;
    }

    //removes all the formatting characters
    char* formatting_string = "\t\n\v\f\r ";
    char* WorkStrPtr = OriginalStr;
    for (int i=0; i<(int)strlen(formatting_string); i++){
        WorkStrPtr = remove_character(WorkStrPtr, formatting_string[i]);
    }

    
    //the entire JSON can be considered just a value, the parse function will do the type inference
    Json* EntireJSONPtr = parse_value(WorkStrPtr);


    return NULL;

}