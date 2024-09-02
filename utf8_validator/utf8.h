// #include <stdlib.h>

typedef struct utf8_string{
    int CodePointCount;
    int len;
    char* RawStrPtr;
} utf8_string;

bool is_valid_raw(char* RawString, int StringLen){
    for(int i=0; i<StringLen;i++){
        if ((unsigned char)RawString[i] >> 3 == 30){
            if (i+3 > StringLen)
                    return false;
            for (int j=1; j<4; j++){
                if ((unsigned char)RawString[i+j] >> 6 != 2)
                    return false;
            }
            i+=3;
        }
        else if ((unsigned char)RawString[i] >> 4 == 14){
            if (i+2 > StringLen)
                    return false;
            for (int j=1; j<3; j++){
                if ((unsigned char)RawString[i+j] >> 6 != 2)
                    return false;
            }
            i+=2;
        }
        else if ((unsigned char)RawString[i] >> 5 == 6){
            i+=1;
            if (i > StringLen)
                return false;
            if ((unsigned char)RawString[i] >> 6 != 2)
                    return false;
        }

        else if ((unsigned char)RawString[i] >> 7 == 0){
        } 
        else return false;
    }

    return true;
}

int get_codepoint_count_raw(char* RawString, int StringLen){
    int CodePointCount = 0;

    for(int i=0; i<StringLen;i++){
        if ((unsigned char)RawString[i] >> 3 == 30){
            if (i+3 > StringLen)
                    return -1;
            for (int j=1; j<4; j++){
                if ((unsigned char)RawString[i+j] >> 6 != 2)
                    return -1;
            }
            CodePointCount += 1;
            i+=3;
        }
        else if ((unsigned char)RawString[i] >> 4 == 14){
            if (i+2 > StringLen)
                    return -1;
            for (int j=1; j<3; j++){
                if ((unsigned char)RawString[i+j] >> 6 != 2)
                    return -1;
            }
            CodePointCount += 1;
            i+=2;
        }
        else if ((unsigned char)RawString[i] >> 5 == 6){
            i+=1;
            if (i > StringLen)
                return -1;
            if ((unsigned char)RawString[i] >> 6 != 2)
                    return -1;
            CodePointCount += 1;
        }


        else if ((unsigned char)RawString[i] >> 7 == 0){
            CodePointCount +=1;
        }
        
        else return -1;
    }

    return CodePointCount;
}

utf8_string* init_utf8_string(utf8_string* StrPointer){
    StrPointer->CodePointCount = 0;
    StrPointer->len = 0;
    StrPointer->RawStrPtr = malloc(1);
    if (StrPointer->RawStrPtr == NULL){
        printf("malloc failed.\n");
        return NULL;
    }

    *(StrPointer->RawStrPtr) = '\0';

    return StrPointer;
}


utf8_string* append_to_str(utf8_string* StrPointer, char* SourcePtr){
    if (StrPointer == NULL || SourcePtr == NULL){
        printf("one of the input pointers were null. returning null...\n");
        return NULL;
    }
    printf("String ptr: %s\nSource ptr: %s\n", StrPointer->RawStrPtr, SourcePtr);

    //allocates a larger string with the StrPointer's raw string value pre-filled, and expanded with the source ptr's length.
    char* NewPtr = realloc(StrPointer->RawStrPtr, StrPointer->CodePointCount+strlen(SourcePtr)+1);
    if (NewPtr == NULL){
        printf("reallocated string was null. returning null...\n");
        return NULL;
    }

    //copies the remaining part of the input string to the newly allocated string.
    if (NULL == strcpy(NewPtr+StrPointer->CodePointCount, SourcePtr)){
        printf("string copying failed.");    
        return NULL;
    }

    //set null byte.
    NewPtr[StrPointer->CodePointCount+strlen(SourcePtr)] = '\0';
    
    StrPointer->RawStrPtr = NewPtr;
    int stringlen = strlen(NewPtr);
    StrPointer->len = stringlen;
    StrPointer->CodePointCount = get_codepoint_count_raw(NewPtr, stringlen);
    if (StrPointer->CodePointCount == -1){
        printf("string is not valid utf8.\n");
        return NULL;
    }
    free(SourcePtr);
    return StrPointer;
}

utf8_string* string_from(char* SourcePtr){
    if (SourcePtr == NULL){
        printf("Source ptr is null. returning null...\n");
        return NULL;
    }

    utf8_string* CreatedString = malloc(sizeof(SourcePtr));
    if (CreatedString == NULL){
        printf("malloc failed");
        return NULL;
    }

    char* NewPtr = calloc(sizeof(SourcePtr)+1, 1);
    if (NULL == strcpy(NewPtr, SourcePtr)){
        printf("string copying failed.");
        return NULL;
    }
    CreatedString->RawStrPtr = NewPtr;
    int stringlen = strlen(NewPtr);
    CreatedString->len = stringlen;
    CreatedString->CodePointCount = get_codepoint_count_raw(NewPtr, stringlen);
    if (CreatedString->CodePointCount == -1){
        printf("string is not valid utf8.\n");
        return NULL;
    }


    
    free(SourcePtr);
    return CreatedString;
}




void free_utf8_string(utf8_string* StrPointer){
    if (StrPointer != NULL){
        if (StrPointer->RawStrPtr != NULL){
            free(StrPointer->RawStrPtr);
        }
        free(StrPointer);
    }
    else printf("StrPointer was null.\n");
}


