// #ifndef extend_with_chars
//     #include "utils.h"
// #endif
#define bool unsigned char
#define false 0
#define true 1
#define ITER_MAX 100000
//#include "typedefs.h"


void Verbose_String_Parsing(char* StringPtr){ //prints bullshit as it goes
    //TODO: create some string checking-handling based on string length
        //null checking
    if (strncmp(StringPtr, (char*)"null", 4) == 0){
        printf("found value is literally null\n");
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




struct JS_Object* String_Parsing(char* StringPtr){ //prints bullshit as it goes
    //TODO: create some string checking-handling based on string length
        //null checking
    struct JS_Object* Main_Obj = malloc(sizeof(struct JS_Object));
    if (strncmp(StringPtr, (char*)"null", 4) == 0){
        printf("found value is a javascript-null\n");
        Main_Obj->Object_Dtype = JS_NULL;
        Main_Obj->Value_Length = 1;
        Main_Obj->Value_s = NULL;
        Main_Obj->Keys = NULL;
        shift_bytes(StringPtr, 4);
        return Main_Obj;
    }
    //number checking
    else if ((StringPtr[0] >= 48 && StringPtr[0] <= 57) || StringPtr[0] == '-'){
        double num;
        int is_num_found = sscanf(StringPtr, "%lf", &num);
        int num_length = 0;
        //a number can only have 2 dots
        int dot_count = 0;
        if (is_num_found)
            printf("found number: %lf\n", num);

        //while all the characters are valid numeric values (or a dot), or(the loop just starts, and the first character is a minus) the number is valid
        
        for(int i=0; ((StringPtr[i] == '-' && i == 0) || (StringPtr[i] >= 48 && StringPtr[i] <= 57) || (StringPtr[i] == '.')) && dot_count < 2;i++){
            if (StringPtr[i] == '.'){
                dot_count++;
            }
            if (dot_count == 2){
                printf("number contains more dots than one. invalid JSON\n");
                free(Main_Obj);
                return NULL;
            }
            num_length++;

            
        }
        
        double* num_storage = malloc(8); //sizeof(double)
        if (num_storage == NULL){
            free(Main_Obj);
            printf("number's storer malloc failed.\n");
            return NULL;
        }
        *num_storage = num;


        printf("number length: %d\n", num_length);
        shift_bytes(StringPtr, num_length);
        Main_Obj->Object_Dtype = NUMBER;
        Main_Obj->Keys = NULL;
        Main_Obj->Value_Length = 1;
        Main_Obj->Value_s = (void*) num_storage;
        return Main_Obj;
    }

    //boolean checking
    else if (strncmp(StringPtr, (char*)"true", 4) == 0){
        printf("found boolean. the value is true\n");
        Main_Obj->Object_Dtype = BOOLEAN;
        Main_Obj->Keys = NULL;
        Main_Obj->Value_Length = 1;
        bool* bool_ptr = malloc(sizeof(bool)); //TODO: make this safe
        *bool_ptr = 1;
        Main_Obj->Value_s = (void*) bool_ptr;
        shift_bytes(StringPtr, 4);
        return Main_Obj;
    }
    else if (strncmp(StringPtr, (char*)"false", 5) == 0){
        printf("found boolean. the value is false\n");
        Main_Obj->Object_Dtype = BOOLEAN;
        Main_Obj->Keys = NULL;
        Main_Obj->Value_Length = 1;
        bool* bool_ptr = malloc(sizeof(bool)); //TODO: make this safe
        *bool_ptr = 0;
        shift_bytes(StringPtr, 5);
        return Main_Obj;
    }
    //string checking
    else if (StringPtr[0] == '"'){
        printf("string is likely found\n");
        int string_len = 0;
        bool is_escaped = false;
        //loop until the string the following is true: string contains a " and is not escaped
        for (int i=1; !(StringPtr[i] == '\"' && !is_escaped); i++){
            if (StringPtr[i] == '\\'){
                printf("next character is an escaped one.\n");
                is_escaped = true;
            }
            else {
                // printf("%c", StringPtr[i]);
                is_escaped = false;
            }
            string_len++;
        }
        // printf("\n");
        char* Parsed_Str = malloc(string_len+1);
        if (Parsed_Str == NULL){
            free(Main_Obj);
            return NULL;
        }
        
        //copy the actual characters (and not the quotation marks only [that's why StringPtr is incremented])
        memcpy(Parsed_Str, StringPtr+1, string_len);
        Parsed_Str[string_len] = '\0';

        printf("string len after calculation: %d\n", string_len);
        
        
        Main_Obj->Object_Dtype = STRING;
        Main_Obj->Keys = NULL;
        Main_Obj->Value_Length = 1;
        Main_Obj->Value_s = Parsed_Str;

        shift_bytes(StringPtr, string_len+2); //+2 because of the " characters
        return Main_Obj;
    }
    //list checking
    else if (StringPtr[0] == '['){
        printf("list is likely found\n");
        struct JS_Object* Sub_Data = malloc(sizeof(struct JS_Object));
        int i = 0;
        bool separator_is_next = false;
        while (*(StringPtr+1) != ']'){ //loop until al the list elements are parsed
            struct JS_Object* Parsed_Data = String_Parsing(StringPtr+1);
            if (Parsed_Data == NULL){
                printf("Parsing one of the list elements failed. Returned NULL\n");
                free(Main_Obj);
                free(Sub_Data);
                return NULL;
            }
            else if ((Parsed_Data == (struct JS_Object*)(-2)) && separator_is_next){ //if it's not null but -2, it can be correct, since it marks a comma
                printf("comma found. That is a legit separator in lists.\n");
                // free(Sub_Data);
                separator_is_next = false;
            }
            else if (Parsed_Data == (struct JS_Object*)(-1)){
                printf("List can't have colons instead of commas!\n");
                free(Main_Obj);
                free(Sub_Data);
                return NULL;
            }
            else if (Parsed_Data == (struct JS_Object*)(-3)){
                printf("General formatter found\n");
                // free(Sub_Data);
            }
            else { //if data is correct
                //in case of first value, no need for realloc
                if (i == 0){
                    printf("parsed data pointer: %p, Sub data pointer: %p\n", Parsed_Data, Sub_Data);
                    Sub_Data->Object_Dtype = Parsed_Data->Object_Dtype;
                    Sub_Data->Keys = Parsed_Data->Keys;
                    Sub_Data->Value_Length = Parsed_Data->Value_Length;
                    Sub_Data->Value_s = Parsed_Data->Value_s;
                    // free(Parsed_Data);
                }
                else {
                    printf("parsed data pointer: %p, Sub data pointer: %p\n", Parsed_Data, Sub_Data);
                    Sub_Data = append_to_JS_object_list(Sub_Data, Parsed_Data, i);
                    if (Sub_Data == NULL){
                        printf("function failed.\n");
                        return NULL;
                    }
                    free(Parsed_Data);
                }
                // printf("parsed data pointer: %p\n", Parsed_Data);
                // free(Parsed_Data);
                
                i++;
                separator_is_next = true;
            }
            
            if (i > ITER_MAX){
                printf("Iteration exceeds %d. Returning...\n", ITER_MAX);
                free(Main_Obj);
                free(Sub_Data);
                return NULL;
            }
        }
        printf("list parsed.\n");
        shift_bytes(StringPtr, 2);
        Main_Obj->Object_Dtype = LIST;
        Main_Obj->Keys = NULL;
        Main_Obj->Value_Length = i;
        Main_Obj->Value_s = (void*)Sub_Data;
        
        return Main_Obj;
    }

    else if (StringPtr[0] == '{'){
        printf("JSON format is likely found\n");
        struct JS_Object* Sub_Data = malloc(sizeof(struct JS_Object));
        char** Keys_List = malloc(sizeof(char*));
        //i is incremented only when encountering values.
        int i = 0;
        bool Key_is_next = true;
        while (*(StringPtr+1) != '}'){
            struct JS_Object* Parsed_Data = String_Parsing(StringPtr+1);
            if (Parsed_Data == NULL){
                printf("string parsing failed inside JSON parser.\n");
                return NULL;
            }
            
            if (Parsed_Data == ((struct JS_Object*)(void*)(-1)) || (struct JS_Object*)(void*)(-2)){
                //TODO: implement normal check for the right formatter.\n
                printf("formatter found in JSON-reader loop.\n");
                continue;
            }
            else if (Parsed_Data == (struct JS_Object*)(-3)){
                printf("General formatter found\n");
                continue;
            }

            else if (Key_is_next){
                if (Parsed_Data->Object_Dtype != STRING){
                    printf("Key would be next, got something with an other type.\n");
                    free(Main_Obj);
                    free(Parsed_Data);
                    return NULL;
                }
                //in the case of the first value, no need for realloc
                if (i == 0){
                    Keys_List[0] = (char*)Parsed_Data->Value_s;
                }
                else {
                    Keys_List = realloc(Keys_List, i+1); //add an extra pointer space
                    Keys_List[i] = (char*)Parsed_Data->Value_s;
                }
                
                // Sub_Data = append_to_JS_object_list(Sub_Data, Parsed_Data, i);
                printf("the next element has to be a colon\n");
                Key_is_next = false;
            }
            else {
                if (i == 0){ //no need for realloc in the first filling of the sub-data
                    Sub_Data->Object_Dtype = Sub_Data->Object_Dtype;
                    Sub_Data->Keys = Parsed_Data->Keys;
                    Sub_Data->Value_Length = Sub_Data->Value_Length;
                    Sub_Data->Value_s = Sub_Data->Value_s;
                }
                else {
                    Sub_Data = append_to_JS_object_list(Sub_Data, Parsed_Data, i);
                    if (Sub_Data == NULL){
                        free(Parsed_Data);
                        free(Main_Obj);
                        printf("Reallocing sub-data failed.\n");
                        return NULL;
                    }
                }
                

                free(Parsed_Data);
                i++;
                Key_is_next = true;
            }
        }
        Main_Obj->Object_Dtype = JSON;
        Main_Obj->Keys = Keys_List;
        Main_Obj->Value_Length = i;
        Main_Obj->Value_s = Sub_Data;
        

        printf("JSON is parsed.\n");
        shift_bytes(StringPtr, 2);


        return Main_Obj;
        
        //TODO: recursively parse string to validate JSON elements (DO NOT overthing)
    }
    else if (StringPtr[0] == ':'){
        free(Main_Obj);
        printf("colon found\n");
        shift_bytes(StringPtr, 1);
        return (struct JS_Object*)((void*)(-1));
    }
    else if (StringPtr[0] == ','){
        free(Main_Obj);
        printf("comma found\n");
        shift_bytes(StringPtr, 1);
        return (struct JS_Object*)((void*)(-2));
    }
    else if (StringPtr[0] == ' ' || StringPtr[0] == '\n' || StringPtr[0] == '\r' || StringPtr[0] == '\t'){
        free(Main_Obj);
        printf("formatter found\n");
        shift_bytes(StringPtr, 1);
        return (struct JS_Object*)((void*)(-3)); //-3 for general formatters: char* formatters = " \n\r\t";
    }
    else {
        printf("impossible token received.\n");
        return NULL;
    }


    return NULL;
}

struct JS_Object* start_string_parsing(char* RawStr){
    char* WorkStr = malloc(strlen(RawStr)+1);
    if (WorkStr == NULL){printf("initial malloc failed.\n");return NULL;};
    strcpy(WorkStr, RawStr);
    // WorkStr = remove_formatters_from_non_strings(RawStr);

    // printf("Raw JSON-string parsed: %s\n", WorkStr);
    // Verbose_String_Parsing(WorkStr);
    struct JS_Object* Parsed_Json = String_Parsing(WorkStr);
    

    return Parsed_Json;
}