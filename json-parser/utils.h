#include "typedefs.h"
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

//returns the first string "slice" in between quotation marks. The string has to start with a "
char* parse_until_quotation_mark(char* StringPartPtr){
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

char* remove_formatters_from_non_strings(char* original){ //remove a list of chars combined in a string
    char* chars_to_remove = " \n\r\t";
    if (original == NULL){
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
    bool is_in_string = false;
    //loop each char through each of the "banned" characters, and append them to a new char array if there is no match in the "banned" character's loop
    for (int i=0; i<original_length; i++){
        for (int j=0; j<removable_chars_len; j++){
            if (original[i] == '"'){
                if (!is_in_string){
                    printf("current char: %c. is_in_string set to true.\n", original[i]);
                    is_in_string = true;
                }
                    
                else{
                    printf("current char: %c. is_in_string set to false.\n", original[i]);
                    is_in_string = false;
                }
            }
            if (original[i] == chars_to_remove[j] && !is_in_string){
                is_char_found = true;
                // printf("\"%c\" matches with \"%c\"\n", original[i], chars_to_remove[j]);
                break;
            }
        }
        if (!is_char_found || is_in_string){

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

//reallocates the Dest_Ptr to append the contents of the Source_Ptr to it. It DOES NOT free Source_Ptr
struct JS_Object* append_to_JS_object_list(struct JS_Object* Dest_Ptr, struct JS_Object* Source_Ptr, int dest_len){
    if (Dest_Ptr == NULL || Source_Ptr == NULL){
        printf("source ptr: %p, dest ptr: %p\n", Dest_Ptr, Source_Ptr);
        return NULL;
    }
    printf("Object List new size: %d\n", dest_len+1);
    struct JS_Object* New_Memblock = realloc(Dest_Ptr, sizeof(struct JS_Object)*(dest_len+1));
    if (New_Memblock == NULL){
        printf("malloc for a larger JS_Object memory block failed.\n");
        return NULL;
    }

    //Increrment the new mem block's ptr by 
    memcpy(New_Memblock+dest_len, Source_Ptr, sizeof(struct JS_Object)); //TODO: check if it's correct (ChatGPT said so)

    return New_Memblock;
}