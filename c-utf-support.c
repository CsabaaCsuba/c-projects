#include <stdio.h>
#include <stdbool.h>

typedef struct {
    char *characters;
    int bytelength;
    int characterlength;
    bool uses_utf8;
} mystring;

void createString(mystring* CreatableString, char *rawString){
    int rawStringSize = 0;
    CreatableString->bytelength = 0;
    CreatableString->characterlength = 0;
    CreatableString->uses_utf8 = 0;
    CreatableString->characters = rawString;

    for(int i=0; *rawString != '\0';i++){
        if (rawString[i] >> 7 == 1){ //if the character is an UTF8 byte (not just ascii)
            CreatableString->uses_utf8 = true;
        }   
        else {
            CreatableString->characterlength += 1;
        }
        CreatableString->bytelength += 1;

        rawString++;
    }

}

