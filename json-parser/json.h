// #ifndef extend_with_chars
//     #include "utils.h"
// #endif
#define bool unsigned char
#define false 0
#define true 1
//#include "typedefs.h"


struct JS_Object* General_Parser(char* StringPtr, int* idx);

struct JS_Object* Parse_Null(char* StringPtr, int* idx){
    if (strncmp(StringPtr, "null", 4) == 0){
        struct JS_Object* return_obj = malloc(sizeof(struct JS_Object));
        if (return_obj == NULL) {
            return NULL;
        }
        return_obj->Keys = NULL;
        return_obj->Value_s = NULL;
        return_obj->Value_Length = 0;
        return_obj->Object_Dtype = JS_NULL;
        *idx = *idx + 4;

        return return_obj;
    }
    else {
        return NULL;
    }
}


struct JS_Object* Parse_Boolean(char* StringPtr, int* idx){
    if (strncmp(StringPtr, "true", 4) == 0){
        struct JS_Object* return_obj = malloc(sizeof(struct JS_Object));
        if (return_obj == NULL) {
            return NULL;
        }
        return_obj->Keys = NULL;
        return_obj->Value_s = malloc(sizeof(bool*));
        if (return_obj->Value_s == NULL){
            free(return_obj);
            return NULL;
        }
        *(bool*)(return_obj->Value_s) = 1;
        return_obj->Value_Length = 1;
        return_obj->Object_Dtype = BOOLEAN;
        *idx = *idx + 4;

        return return_obj;
    }
    else if (strncmp(StringPtr, "false", 5) == 0){
        struct JS_Object* return_obj = malloc(sizeof(struct JS_Object));
        if (return_obj == NULL) {
            return NULL;
        }
        return_obj->Keys = NULL;
        return_obj->Value_s = malloc(sizeof(bool*));
        if (return_obj->Value_s == NULL){
            free(return_obj);
            return NULL;
        }
        *(bool*)(return_obj->Value_s) = 0;
        return_obj->Value_Length = 1;
        return_obj->Object_Dtype = BOOLEAN;
        *idx = *idx + 5;

        return return_obj;
    }
    else {
        return NULL;
    }

}


struct JS_Object* Parse_Number(char* StringPtr, int* idx){
    double* num_storage = malloc(sizeof(double));
    if (num_storage == NULL){
        return NULL;
    }
    struct JS_Object* return_obj = malloc(sizeof(struct JS_Object));
    if (return_obj == NULL){
        return NULL;
    }

    int is_num_found = sscanf(StringPtr, "%lf", num_storage);
    
    if (!is_num_found){
        printf("No legit number was found\n");
        free(num_storage);
        free(return_obj);
        return NULL;
    }

    //num_len is 1 because the first number is guaranteed to be valid (because if it wasn't valid)
    int num_len = 1;
    for(;(StringPtr[num_len] >= 48 && StringPtr[num_len] <= 57) || (StringPtr[num_len] == '.');num_len++){
        //counts the length of the number via the loop
    }
    
    return_obj->Object_Dtype = NUMBER;
    return_obj->Value_s = (void*) num_storage;
    return_obj->Keys = NULL;
    return_obj->Value_Length = 1;
    *idx = *idx + num_len;
    return return_obj;
}

//StringPtr should start at "
struct JS_Object* Parse_String(char* StringPtr, int* idx){
    *idx = *idx + 1;
    StringPtr += 1;
    struct JS_Object* return_obj = malloc(sizeof(struct JS_Object));
    if (return_obj == NULL) {
        return NULL;
    }
    int str_len = 0;
    bool escaped = false;
    while (StringPtr[str_len] != '"' || escaped){ //it should end when the string is closed AND isn't escaped
        if (StringPtr[str_len] == '\\'){
            escaped = true;
        }
        if (StringPtr[str_len] == '"'){
            escaped = false;
        }
        if (StringPtr[str_len] == '\0'){
            printf("String abruptly ended.\n");
            free(return_obj);
            return NULL;
        }
        str_len ++;
    }
    return_obj->Value_s = malloc(str_len+1);
    if (return_obj->Value_s == NULL){
        free(return_obj);
        return NULL;
    }

    strncpy(return_obj->Value_s, StringPtr, str_len);
    *(char*)(return_obj->Value_s+str_len) = '\0'; //null termination guaranteed

    *idx = *idx + str_len + 1; //to count in the ending char

    return_obj->Object_Dtype = STRING;
    return_obj->Keys = NULL;
    return_obj->Value_Length = 1;
    
    return return_obj;
}

//StringPtr should be the start of the string (because of recursion)
struct JS_Object* Parse_Array(char* StringPtr, int* idx){
    struct JS_Object* return_obj = malloc(sizeof(struct JS_Object));
    if (return_obj == NULL) {
        return NULL;
    }
    *idx = *idx + 1; //to skip the [ character

    return_obj->Value_Length = 0;

    while (StringPtr[*idx] != ']'){
        if (StringPtr[*idx] == '\0'){
            printf("Array abruptly ended.\n");
            free(return_obj);
            return NULL;
        }
        struct JS_Object* found_element = General_Parser(StringPtr, idx);
        if (found_element == NULL){
            if (return_obj->Value_Length == 0){
                printf("Empty Array found\n");
                return_obj->Value_s = NULL; //list will be empty
                break;
            }
        }
        return_obj->Value_Length += 1;
        //if it's the first found value, allocate memory as well
        if (return_obj->Value_Length == 1){
            return_obj->Value_s = malloc(sizeof(struct JS_Object));
            if (return_obj->Value_s == NULL){
                free(return_obj);
                return NULL;
            }
            memcpy(return_obj->Value_s, found_element, sizeof(struct JS_Object));
        }
        //else, expand the memory block
        else {
            return_obj->Value_s = realloc(return_obj->Value_s, return_obj->Value_Length * sizeof(struct JS_Object));
            if (return_obj->Value_s == NULL){
                free(return_obj);
                return NULL;
            }
            //if value_length is 2, copy to index 1 (the second element)
            memcpy(return_obj->Value_s + return_obj->Value_Length-1, found_element, sizeof(struct JS_Object));
        }
        //because the Object is copied into the Value_s array, this can be freed
        free(found_element);

        //TODO: iterate for whitespaces
        while (StringPtr[*idx] == ' ' || StringPtr[*idx] == '\r' || StringPtr[*idx] == '\n' || StringPtr[*idx] == '\t'){
            *idx = *idx + 1;
        }
        //if next element is not a "," or the end of the array, error out

        if (StringPtr[*idx] == ','){
            *idx = *idx + 1;
        }
        else if (StringPtr[*idx] == ']'){
            *idx = *idx + 1;
            printf("End of array found.\n");
            break;
        }
        else {
            printf("illegal character: %d (ascii)\n", StringPtr[*idx]);
            free(return_obj->Value_s);
            free(return_obj);
            return NULL;
        }        
    }

    return_obj->Keys = NULL;
    return_obj->Object_Dtype = LIST;
    return return_obj;
}

//because of recursion, StringPtr is always the start of the string
struct JS_Object* Parse_Object(char* StringPtr, int* idx){
    struct JS_Object* return_obj = malloc(sizeof(struct JS_Object));
    return_obj->Keys = NULL;
    return_obj->Object_Dtype = JSON;
    return_obj->Value_s = NULL;
    return_obj->Value_Length = 0;
    if (return_obj == NULL) {
        return NULL;
    }
    *idx = *idx + 1;
    while (StringPtr[*idx] != '}'){
        struct JS_Object* key_str = Parse_String(StringPtr, idx);
        if (key_str == NULL){
            printf("no valid string found as key");
            free(return_obj);
            return NULL;
        }
        return_obj->Value_Length++;
        //if this is the first key, allocate storage for the Keys
        if (return_obj->Value_Length == 1){
            return_obj->Keys = malloc(sizeof(char*));
            if (return_obj->Keys == NULL){
                //TODO: create freeing function
                free(key_str->Value_s);
                free(key_str);
                free(return_obj);
                return NULL;
            }
        }
        //else, just expand the array of char*
        else {
            return_obj->Keys = realloc(return_obj->Keys, return_obj->Value_Length * sizeof(char*));
            if (return_obj->Keys == NULL){
                //TODO: create freeing function
                free(key_str->Value_s);
                free(key_str);
                free(return_obj);
                return NULL;
            }
        }
        return_obj->Keys[return_obj->Value_Length-1] = key_str->Value_s;
        free(key_str);

        

        //skip whitespaces until :
        while (StringPtr[*idx] == ' ' || StringPtr[*idx] == '\r' || StringPtr[*idx] == '\n' || StringPtr[*idx] == '\t'){
            *idx = *idx + 1;
        }
        if (StringPtr[*idx] == ':'){
            printf("Object delimiter found\n");
            *idx = *idx + 1;
        }
        else {
            printf("no valid delimiter (\":\") found.\n");
            free(return_obj);
            return NULL;
        }
        //get the actual object.
        struct JS_Object* found_element = General_Parser(StringPtr, idx);
        if (found_element == NULL){
            if (return_obj->Value_Length == 0){
                printf("Empty Array found\n");
                return_obj->Value_s = NULL; //list will be empty
                break;
            }
        }
        return_obj->Value_Length += 1;
        //if it's the first found value, allocate memory
        if (return_obj->Value_Length == 1){
            return_obj->Value_s = malloc(sizeof(struct JS_Object));
            if (return_obj->Value_s == NULL){
                free(return_obj);
                return NULL;
            }
        }
        //else, expand the memory block
        else {
            return_obj->Value_s = realloc(return_obj->Value_s, return_obj->Value_Length * sizeof(struct JS_Object));
            if (return_obj->Value_s == NULL){
                free(return_obj);
                return NULL;
            }
        }
        memcpy(return_obj->Value_s + return_obj->Value_Length-1, found_element, sizeof(struct JS_Object));
        //because the Object is copied into the Value_s array, this can be freed
        free(found_element);

        while (StringPtr[*idx] == ' ' || StringPtr[*idx] == '\r' || StringPtr[*idx] == '\n' || StringPtr[*idx] == '\t'){
            *idx = *idx + 1;
        }
        //if next element is not a "," or the end of the array, error out

        if (StringPtr[*idx] == ','){
            *idx = *idx + 1;
            printf("Comma found, jumping to new line.\n");
        }
        else if (StringPtr[*idx] == '}'){
            *idx = *idx + 1;
            printf("End of object found.\n");
            break;
        }
        else {
            printf("illegal character: %d (ascii)\n", StringPtr[*idx]);
            free(return_obj->Value_s);
            free(return_obj);
            return NULL;
        }     

    }
    return return_obj;
}

//because of recursion, StringPtr is always the start of the string
struct JS_Object* General_Parser(char* StringPtr, int* idx){ //prints bullshit as it goes
    if (StringPtr[*idx] == '\0'){
        printf("there's nothing to parse.\n");
        return NULL;
    }
    while (StringPtr[*idx] == ' ' || StringPtr[*idx] == '\r' || StringPtr[*idx] == '\n' || StringPtr[*idx] == '\t'){
        *idx = *idx + 1;
    }
    if (StringPtr[*idx] == 'n'){
        struct JS_Object* return_obj = Parse_Null(StringPtr+(*idx), idx);
        if (return_obj == NULL){
            printf("Expected javascript null, found nothing legit\n");
            return NULL;
        }
        return return_obj;
    }
    else if (StringPtr[*idx] == 't' || StringPtr[*idx] == 'f'){
        struct JS_Object* return_obj = Parse_Boolean(StringPtr+(*idx), idx);
        if (return_obj == NULL){
            printf("Expected boolean value, found nothing legit\n");
            return NULL;
        }
        return return_obj;
    }
    else if (StringPtr[*idx] == '-' || (StringPtr[*idx] >= '0' && StringPtr[*idx] <= '9')) {
        struct JS_Object* return_obj = Parse_Number(StringPtr+(*idx), idx);
        if (return_obj == NULL){
            printf("Expected Number, found nothing legit\n");
            return NULL;
        }
        return return_obj;
    }
    else if (StringPtr[*idx] == '[') {
        struct JS_Object* return_obj = Parse_Array(StringPtr+(*idx), idx);
        if (return_obj == NULL){
            printf("Expected Javascript Array, found nothing legit\n");
            return NULL;
        }
        return return_obj;
    }
    else if (StringPtr[*idx] == '{'){
        struct JS_Object* return_obj = Parse_Object(StringPtr+(*idx), idx);
        if (return_obj == NULL){
            printf("Expected Javascript Object, found nothing legit\n");
            return NULL;
        }
        return return_obj;
    }
    else {
        printf("Character %d (ascii) is not legit here.\n", StringPtr[*idx]);
        return NULL;
    }
}



struct JS_Object* start_string_parsing(char* RawStr){
    char* WorkStr = malloc(strlen(RawStr)+1);
    if (WorkStr == NULL){printf("initial malloc failed.\n");return NULL;};
    strcpy(WorkStr, RawStr);
    // WorkStr = remove_formatters_from_non_strings(RawStr);

    // printf("Raw JSON-string parsed: %s\n", WorkStr);
    // Verbose_String_Parsing(WorkStr);
    int i = 0;
    struct JS_Object* Parsed_Json = General_Parser(WorkStr, &i);
    

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
    // printf("Objects' Dtype: %d\n", InputObj->Object_Dtype);
    
    switch (InputObj->Object_Dtype){
        case JS_NULL:
            ReturnVal = malloc(5);
            strcpy(ReturnVal, "null");
            return ReturnVal;
        case NUMBER:
            //TODO: write a function which converts the number to a string of it's actual shortest length (without any trailing 0s)

            data_len = snprintf(NULL, 0, "%lf", *(double*)InputObj->Value_s); 
            ReturnVal = malloc(data_len+1);
            snprintf(ReturnVal, data_len, "%lf", *(double*)InputObj->Value_s);
            // printf("parsed number: %s\n", ReturnVal);
            return ReturnVal;
        case BOOLEAN:
            // printf("Objects' Value: %d\n", *(bool*)InputObj->Value_s);
            // printf("%d\n", *(bool*)InputObj->Value_s);
            if (*(bool*)InputObj->Value_s == 0){
                ReturnVal = malloc(6);
                strcpy(ReturnVal, "false");
            }
            else {
                ReturnVal = malloc(5);
                strcpy(ReturnVal, "true");
            }
            // printf("received boolean: %s\n", ReturnVal);
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
            // printf("JSON to string conversion started.\n");
        //TODO: check if the keys & values are valid
            // printf("key: %s\n", InputObj->Keys[i]);
            Return_Str = extend_with_chars(Return_Str, "\""); //starting quotation mark
            Return_Str = extend_with_chars(Return_Str, InputObj->Keys[i]);
            Return_Str = extend_with_chars(Return_Str, "\""); //ending quotation mark
            Return_Str = extend_with_chars(Return_Str, " : ");
            
            
            struct JS_Object* Current_Obj = ((struct JS_Object* )InputObj->Value_s)+i;
            // printf("type of js object: %d\n", Current_Obj->Object_Dtype);
            char* Obj_Str = Convert_To_String(Current_Obj);
            Return_Str = extend_with_chars(Return_Str, Obj_Str);
            free(Obj_Str);

            if (i < InputObj->Value_Length-1)
                Return_Str = extend_with_chars(Return_Str, ", ");

        }

        Return_Str = extend_with_chars(Return_Str, "}");
    }
    else if (InputObj->Object_Dtype == LIST){
        // printf("list to string conversion started.\n");
        Return_Str = Convert_List_To_String(InputObj);
    }
    else {
        // printf("primitive to string conversion started.\n");
        Return_Str = Convert_Primitive_To_String(InputObj);
    }
    return Return_Str;
}