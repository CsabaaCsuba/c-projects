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
    // struct JS_Object** Sub_Values; //an array of pointers, which have the type JS_Object (the last 8 continous 0 bytes stop the pointer list)
    void* Sub_Value //pointers will always be casted to the right data type

} JS_Object;

void Verbose_String_Parsing(char* StringPtr){ //prints bullshit as it goes
    //TODO: create some string checking-handling based on string length
        //null checking
    if (strncmp(StringPtr, (char*)"null", 4) == 0){
        printf("found is literally null\n");
        shift_bytes(StringPtr, 4);
    }
    //number checking
    else if (StringPtr[0] >= 48 && StringPtr[0] <= 57){
        double num;
        int is_num_found = sscanf(StringPtr, "%lf", &num);
        int num_length = 0;
        //a number can only have 2 dots
        int dot_count = 0;
        if (is_num_found)
            printf("found number: %lf\n", num);

        //while all the characters are valid numeric values (or a dot), the number is valid
        
        for(int i=0; ((StringPtr[i] >= 48 && StringPtr[i] <= 57) || (StringPtr[i] == '.')) && dot_count < 2;i++){
            if (StringPtr[i] == '.'){
                dot_count++;
            }
            if (dot_count == 2){
                printf("number contains more dots than one. invalid JSON\n");
                return;
            }
            num_length++;

            
        }
        
        printf("number length: %d\n", num_length);
        shift_bytes(StringPtr, num_length);

    }

    //boolean checking
    else if (strncmp(StringPtr, (char*)"true", 4) == 0){
        printf("found boolean. the value is true\n");
        shift_bytes(StringPtr, 4);
    }
    else if (strncmp(StringPtr, (char*)"false", 5) == 0){
        printf("found boolean. the value is false\n");
        shift_bytes(StringPtr, 5);
    }
    //string checking
    else if (StringPtr[0] == '"'){
        printf("string is likely found\n");
        //TODO: write a string-parser function, because of the \" character
        char* StringPartEndPtr = strchr(StringPtr+1, '"');
        int str_len = (int) (StringPartEndPtr - StringPtr);
        shift_bytes(StringPtr, str_len+1); //+1 because of the initial " character
        //TODO: check if entire string is valid
    }
    //list checking
    else if (StringPtr[0] == '['){
        printf("list is likely found\n");
        // int i = 0;
        while (*(StringPtr+1) != ']'){ //loop until al the numbers are parsed
            Verbose_String_Parsing(StringPtr+1);
        }
        printf("list parsed.\n");
        shift_bytes(StringPtr, 2);
        // if (StringPtr+1);
        
        //TODO: check list's elements
    }

    else if (StringPtr[0] == '{'){
        printf("JSON format is likely found\n");
        while (*(StringPtr+1) != '}'){
            Verbose_String_Parsing(StringPtr+1);
        }
        printf("JSON is parsed.\n");
        shift_bytes(StringPtr, 2);
        
        //TODO: recursively parse string to validate JSON elements (DO NOT overthing)
    }
    else if (StringPtr[0] == ':'){
        printf("colon found\n");
        shift_bytes(StringPtr, 1);
    }
    else if (StringPtr[0] == ','){
        printf("comma found\n");
        shift_bytes(StringPtr, 1);
    }
    else {
        printf("impossible token received.\n");
    }
}


JS_Object* Verbose_String_Parsing(char* StringPtr){ //prints bullshit as it goes
    //TODO: create some string checking-handling based on string length
        //null checking
    JS_Object* Main_Obj = malloc(sizeof(Main_Obj));
    if (strncmp(StringPtr, (char*)"null", 4) == 0){
        printf("found is literally null\n");
        Main_Obj->Object_Dtype = JS_NULL;
        Main_Obj->Value_Length = 1;
        Main_Obj->Values = NULL;
        Main_Obj->Keys = NULL;
        shift_bytes(StringPtr, 4);
    }
    //number checking
    else if (StringPtr[0] >= 48 && StringPtr[0] <= 57){
        double num;
        int is_num_found = sscanf(StringPtr, "%lf", &num);
        int num_length = 0;
        //a number can only have 2 dots
        int dot_count = 0;
        if (is_num_found)
            printf("found number: %lf\n", num);

        //while all the characters are valid numeric values (or a dot), the number is valid
        
        for(int i=0; ((StringPtr[i] >= 48 && StringPtr[i] <= 57) || (StringPtr[i] == '.')) && dot_count < 2;i++){
            if (StringPtr[i] == '.'){
                dot_count++;
            }
            if (dot_count == 2){
                printf("number contains more dots than one. invalid JSON\n");
                return;
            }
            num_length++;

            
        }
        
        printf("number length: %d\n", num_length);
        shift_bytes(StringPtr, num_length);

    }

    //boolean checking
    else if (strncmp(StringPtr, (char*)"true", 4) == 0){
        printf("found boolean. the value is true\n");
        shift_bytes(StringPtr, 4);
    }
    else if (strncmp(StringPtr, (char*)"false", 5) == 0){
        printf("found boolean. the value is false\n");
        shift_bytes(StringPtr, 5);
    }
    //string checking
    else if (StringPtr[0] == '"'){
        printf("string is likely found\n");
        //TODO: write a string-parser function, because of the \" character
        char* StringPartEndPtr = strchr(StringPtr+1, '"');
        int str_len = (int) (StringPartEndPtr - StringPtr);
        shift_bytes(StringPtr, str_len+1); //+1 because of the initial " character
        //TODO: check if entire string is valid
    }
    //list checking
    else if (StringPtr[0] == '['){
        printf("list is likely found\n");
        // int i = 0;
        while (*(StringPtr+1) != ']'){ //loop until al the numbers are parsed
            Verbose_String_Parsing(StringPtr+1);
        }
        printf("list parsed.\n");
        shift_bytes(StringPtr, 2);
        // if (StringPtr+1);
        
        //TODO: check list's elements
    }

    else if (StringPtr[0] == '{'){
        printf("JSON format is likely found\n");
        while (*(StringPtr+1) != '}'){
            Verbose_String_Parsing(StringPtr+1);
        }
        printf("JSON is parsed.\n");
        shift_bytes(StringPtr, 2);
        
        //TODO: recursively parse string to validate JSON elements (DO NOT overthing)
    }
    else if (StringPtr[0] == ':'){
        printf("colon found\n");
        shift_bytes(StringPtr, 1);
    }
    else if (StringPtr[0] == ','){
        printf("comma found\n");
        shift_bytes(StringPtr, 1);
    }
    else {
        printf("impossible token received.\n");
    }
}

JS_Object* start_string_parsing(char* RawStr){
    char* WorkStr;
    WorkStr = remove_characters(RawStr, (char*)" \n\r\t");

    printf("string parsed: %s\n", WorkStr);
    Verbose_String_Parsing(WorkStr);
    

    return NULL;
}