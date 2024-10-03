#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "json.h"

int main(){
    
    char* json_test_str = malloc(125);
    strncpy(json_test_str,
    "{\"name\":\"Alice\", \"age\":30, \"isStudent\":false, \"grades\":[85.5, 90, null], \"address\":{ \"city\":\"Wonderland\", \"zipcode\":12345 }}",
    125
    );
    json_test_str[124] = '\0';

    printf("started: %s\n", json_test_str);
    start_string_parsing(json_test_str);
    return 0;
}