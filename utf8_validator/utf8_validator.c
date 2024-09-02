#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utf8.h"



int main(){
    char InvalidString[] = "\xf0\x28\x8c\x28";
    printf("validator returned: %d\n", is_valid_raw((char*)&InvalidString, strlen(InvalidString)));

    char ValidString[] = "𐌼";
    printf("validator returned: %d\n", is_valid_raw((char*)&ValidString, strlen(ValidString)));
    
    
    utf8_string MyString;
    init_utf8_string(&MyString);
    append_to_str(&MyString, "Halohalo");
    append_to_str(&MyString, "bruh");

    printf("current string: %s\n", MyString.RawStrPtr);
    
    
    utf8_string* MyStringPtr2 = string_from("hiiii");
    printf("string 2: %s\n", MyStringPtr2->RawStrPtr);

    // char* fasz = "す";
    printf("Codepoint count: %d\n", get_codepoint_count_raw("ŰŐすa", strlen("ŰŐすa")));

    utf8_string InputString;
    init_utf8_string(&InputString);
    char input_buff[BUFSIZ];

    while (1 == 1){
        scanf("%512s", input_buff);
        if (strcmp((char*)&input_buff, "n") == 0){
            break;
        }
        append_to_str(&InputString, (char*)&input_buff);
    }
    printf("Input string at the end: %s", InputString.RawStrPtr);

    // free_utf8_string(&InputString);

    return 0;
}