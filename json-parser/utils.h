
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