// #ifndef extend_with_chars
//     #include "utils.h"
// #endif
#define bool unsigned char
#define false 0
#define true 1

typedef enum DataType {
    JS_NULL,
    NUMBER,
    BOOLEAN,
    STRING,
    LIST,
    JSON
} DataType;

struct JS_Object;

typedef struct JS_Object{
    //the variables are called current because references could be possible to lower values
    DataType Object_Dtype;
    char** Keys; //can be null (when it's not of JSON type), but can be an array of char pointers too

    unsigned int Value_Length; //in case of a list, represents the amount of Keys
    struct JS_Object** Values; //an array of pointers, which have the type JS_Object (the last 8 continous 0 bytes stop the pointer list)


} JS_Object;

void Parse_String(char* StringPtr){ //prints bullshit as it goes
    //TODO: create some string checking-handling based on string length
    if (strncmp(StringPtr, (char*)"null", 5) == 0){ //if strings are the same;
        printf("found null.\n");
    }
    else if (StringPtr[0] >= 48 && StringPtr[0] <= 57){
        printf("found number.\n");
    }
    else if (strncmp(StringPtr, (char*)"true", 5) == 0|| strncmp(StringPtr, (char*)"false", 6) == 0){
        printf("found boolean.\n");
    }
    else if (StringPtr[0] == '"'){
        printf("string is likely found\n");
        char* StringPartEndPtr = strchr(StringPtr+1, '"');
        int str_len = (int) (StringPartEndPtr - StringPtr);
        shift_bytes(StringPtr, str_len+1); //+1 because of the initial " character
        //TODO: check if entire string is valid
    }
    else if (StringPtr[0] == '['){
        printf("list is likely found\n");
        
        //TODO: check list's elements
    }
    else if (StringPtr[0] == '{'){
        printf("JSON format is likely found\n");
        int i = 10;
        while (*(StringPtr+1) != '\0' || i > 10){
            Parse_String(StringPtr+1);
        }
        
        //TODO: recursively parse string to validate JSON elements (DO NOT overthing)
    }
    else if (StringPtr[0] == ':'){
        printf("delimiter found\n");
        shift_bytes(StringPtr, 1);
    }
    else if (StringPtr[0] == ','){
        printf("comma found");
        shift_bytes(StringPtr, 1);
    }
}

JS_Object* start_string_parsing(char* RawStr){
    char* WorkStr = RawStr;
    char whitespaces[] = " \n\r\t";
    printf("bruh.\n");
    for (int i=0; i<5; i++){
        //TODO: this doesn't work, check the function
        WorkStr = remove_character(WorkStr, whitespaces[i]);
    }
    printf("string parsed: %s: \n", WorkStr);
    Parse_String(WorkStr);
    

    return NULL;
}