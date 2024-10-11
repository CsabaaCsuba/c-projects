// #ifndef extend_with_chars
//     #include "utils.h"
// #endif
#define bool unsigned char
#define false 0
#define true 1
#define ITER_MAX 100000
//#include "typedefs.h"




struct JS_Object* String_Parsing(char* StringPtr){ //prints bullshit as it goes
    if (StringPtr == NULL){
        printf("Main parser function's pointer was null.\n");
        return NULL;
    }
    //TODO: create some string checking-handling based on string length
    //null checking
    struct JS_Object* Main_Obj = malloc(sizeof(struct JS_Object));
    Main_Obj->Value_Length = 1;
    char** Keys_Ptr = NULL;
    if (strncmp(StringPtr, (char*)"null", 4) == 0){
        printf("found value is a javascript-null\n");
        Main_Obj->Object_Dtype = JS_NULL;
        Main_Obj->Value_s = NULL;
        shift_bytes(StringPtr, 4);
    }
    //if current character is numeric / starts with a number
    else if ((StringPtr[0] >= 48 && StringPtr[0] <= 57) || StringPtr[0] == '-'){
        double* num_storage = malloc(8);
        if (num_storage == NULL){
            free(Main_Obj);
            printf("number's storer malloc failed.\n");
            return NULL;
        }
        int is_num_found = sscanf(StringPtr, "%lf", num_storage);
        int num_length = 0;
        //a number can only have 2 dots
        int dot_count = 0;
        if (!is_num_found){
            printf("There was only a minus sign, but not a number after it.\n");
            free(Main_Obj);
            return NULL;
        }


        //NOT NEEDED
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
        
        shift_bytes(StringPtr, num_length);
        Main_Obj->Object_Dtype = NUMBER;
        Main_Obj->Value_s = (void*) num_storage;
    }

    //boolean checking
    else if (strncmp(StringPtr, (char*)"true", 4) == 0){
        printf("found boolean. the value is true\n");
        Main_Obj->Object_Dtype = BOOLEAN;
        bool* bool_ptr = malloc(sizeof(bool)); //TODO: make this safe
        *bool_ptr = 1;
        Main_Obj->Value_s = (void*) bool_ptr;
        shift_bytes(StringPtr, 4);
    }
    else if (strncmp(StringPtr, (char*)"false", 5) == 0){
        printf("found boolean. the value is false\n");
        Main_Obj->Object_Dtype = BOOLEAN;
        bool* bool_ptr = malloc(sizeof(bool)); //TODO: make this safe
        *bool_ptr = 0;
        Main_Obj->Value_s = (void*) bool_ptr;
        shift_bytes(StringPtr, 5);
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
        Main_Obj->Value_s = (void*)Parsed_Str;
        printf("string found: %s\n", (char*)Main_Obj->Value_s);

        shift_bytes(StringPtr, string_len+2); //+2 because of the " characters
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
        Main_Obj->Value_Length = i;
        Main_Obj->Value_s = (void*)Sub_Data;
        
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
            if (Parsed_Data == ((struct JS_Object*)(void*)(-1)) || Parsed_Data == (struct JS_Object*)(void*)(-2)){
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
                    printf("prev Key: %s\n", ((char**)Keys_List)[i-1]);
                    printf("next size: %d\n", i+1);
                    Keys_List = realloc(Keys_List, i*sizeof(char*)+sizeof(char*)); //add an extra pointer space
                    printf("re-bruh alloc\n");

                    Keys_List[i] = (char*)Parsed_Data->Value_s;
                }
                
                // Sub_Data = append_to_JS_object_list(Sub_Data, Parsed_Data, i);
                printf("the next element has to be a colon\n");
                Key_is_next = false;
            }
            else {
                if (i == 0){ //no need for realloc in the first filling of the sub-data
                    Sub_Data->Object_Dtype = Parsed_Data->Object_Dtype;
                    Sub_Data->Keys = Parsed_Data->Keys;
                    Sub_Data->Value_Length = Parsed_Data->Value_Length;
                    Sub_Data->Value_s = Parsed_Data->Value_s;
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
        Keys_Ptr = Keys_List;
        Main_Obj->Value_Length = i;
        Main_Obj->Value_s = Sub_Data;
        shift_bytes(StringPtr, 2);        
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

    Main_Obj->Keys = Keys_Ptr;

    return Main_Obj;
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

char* Convert_To_String(struct JS_Object* InputObj);


char* Convert_Primitive_To_String(struct JS_Object* InputObj){
    char* ReturnVal;
    int data_len;
    if (InputObj == NULL){
        printf("null\n");
        return NULL;
    }
    printf("Objects' Dtype: %d\n", InputObj->Object_Dtype);
    
    switch (InputObj->Object_Dtype){
        case JS_NULL:
            ReturnVal = malloc(5);
            strcpy(ReturnVal, "null");
            return ReturnVal;
        case NUMBER:
            data_len = snprintf(NULL, 0, "%lf", *(double*)InputObj->Value_s); 
            ReturnVal = malloc(data_len+1);
            snprintf(ReturnVal, data_len, "%lf", *(double*)InputObj->Value_s);
            printf("parsed number: %s\n", ReturnVal);
            return ReturnVal;
        case BOOLEAN:
            printf("Objects' Value: %d\n", *(bool*)InputObj->Value_s);
            printf("%d\n", *(bool*)InputObj->Value_s);
            if (*(bool*)InputObj->Value_s == 0){
                ReturnVal = malloc(6);
                strcpy(ReturnVal, "false");
            }
            else {
                ReturnVal = malloc(5);
                strcpy(ReturnVal, "true");
            }
            printf("received boolean: %s\n", ReturnVal);
            return ReturnVal;
        case STRING:
            data_len = strlen((char*)InputObj->Value_s) + 3; //+3 for the 2 quotation marks and the null-terminator
            ReturnVal = malloc(data_len+3);
            ReturnVal[0] = '"';
            strncpy(ReturnVal+1, InputObj->Value_s, data_len-2);
            ReturnVal[data_len-2] = '"';
            ReturnVal[data_len-1] = '\0';
            return ReturnVal;
        default:
            printf("non-primitive dtype.\n");
            return NULL;
    };

}

char* Convert_List_To_String(struct JS_Object* List_Object){
    if (List_Object->Object_Dtype != LIST){
        printf("won't convert non-lists to lists\n");
        return NULL;
    }
    char* Return_Str = malloc(2);
    strcpy(Return_Str, "[");
    // int return_len = 0;
    for (unsigned int i=0; i<List_Object->Value_Length; i++){
        char* CurrentElementStr = Convert_To_String(((struct JS_Object*)List_Object->Value_s+i));
        if (CurrentElementStr == NULL){
            printf("List's element's conversion failed.\n");
            return NULL;
        }
        Return_Str = extend_with_chars(Return_Str, CurrentElementStr);

        //add a comma if there are other list elements
        if (i < List_Object->Value_Length-1)
            Return_Str = extend_with_chars(Return_Str, ", ");

        //call the convert_to_string on each one (that will redirect to Convert_Primitive_To_String as well)
    }

    Return_Str = extend_with_chars(Return_Str, "]");

    return Return_Str;
}




//this handles JSON-type objects
char* Convert_To_String(struct JS_Object* InputObj){
    char* Return_Str;
    if (InputObj->Object_Dtype == JSON){
        Return_Str = malloc(3);
        strcpy(Return_Str, "{");
        for (unsigned int i=0; i<InputObj->Value_Length; i++){
            printf("JSON to string conversion started.\n");
            //TODO: check if the keys & values are valid
            printf("key: %s\n", InputObj->Keys[i]);
            Return_Str = extend_with_chars(Return_Str, "\""); //starting quotation mark
            Return_Str = extend_with_chars(Return_Str, InputObj->Keys[i]);
            Return_Str = extend_with_chars(Return_Str, "\""); //ending quotation mark
            Return_Str = extend_with_chars(Return_Str, " : ");
            
            
            struct JS_Object* Current_Obj = ((struct JS_Object* )InputObj->Value_s)+i;
            printf("type of js object: %d\n", Current_Obj->Object_Dtype);
            char* Obj_Str = Convert_To_String(Current_Obj);
            Return_Str = extend_with_chars(Return_Str, Obj_Str);
            free(Obj_Str);

            if (i < InputObj->Value_Length-1)
                Return_Str = extend_with_chars(Return_Str, ", ");

        }

        Return_Str = extend_with_chars(Return_Str, "}");
    }
    else if (InputObj->Object_Dtype == LIST){
        printf("list to string conversion started.\n");
        Return_Str = Convert_List_To_String(InputObj);
    }
    else {
        printf("primitive to string conversion started.\n");
        Return_Str = Convert_Primitive_To_String(InputObj);
    }
    return Return_Str;
}