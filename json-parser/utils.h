#define bool unsigned char
#define false 0
#define true 1

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

char* remove_characters(char* original, char* chars_to_remove){ //remove a list of chars combined in a string
    if (original == NULL || chars_to_remove == NULL){
        return NULL;
    }
    int original_length = strlen(original);
    if (original_length == 0) return NULL;

    char* NewArr = malloc(original_length+1);
    if (NewArr == NULL) return NULL;

    int removable_chars_len = strlen(chars_to_remove);
    if (removable_chars_len == 0) return NULL;
    printf("chars len: %d\n", removable_chars_len);

    int new_strlen = 0;
    bool is_char_found = false;
    //loop each char through each of the "banned" characters, and append them to a new char array if there is no match in the "banned" character's loop
    for (int i=0; i<original_length; i++){
        for (int j=0; j<removable_chars_len; j++){
            if (original[i] == chars_to_remove[j]){
                is_char_found = true;
                // printf("\"%c\" matches with \"%c\"\n", original[i], chars_to_remove[j]);
                break;
            }
        }
        if (!is_char_found){
            NewArr[new_strlen] = original[i];
            new_strlen++;
        }
        is_char_found = false;
    }
        
    
    NewArr[new_strlen] = '\0';
    NewArr = realloc(NewArr, new_strlen+1);
    // original = realloc(original, new_strlen+1);
    // strcpy(original, NewArr);
    // original[new_strlen] = '\0';
    if (NewArr == NULL) return NULL;

    return NewArr;

}

void shift_bytes(char* str, int amount_to_shift) {
    int len = strlen(str);
    if (amount_to_shift >= len) {
        return;
    }
    memmove(str, str + amount_to_shift, len - amount_to_shift + 1); // +1 for null terminator
}