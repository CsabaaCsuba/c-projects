#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

int check_if_string_is_closed(char* StringFirstQuotePtr){ //goes from first quote to last
    if (StringFirstQuotePtr == NULL || StringFirstQuotePtr[0] != '\"'){
        printf("Wrong pointer or string pointer isn't positioned correctly.\n");
        return -1;
    }
    for (int i=1; i<strlen(StringFirstQuotePtr); i++){
        if (StringFirstQuotePtr[i] == '\"' && StringFirstQuotePtr[i-1] != '\\'){
            return i;
        }
    }
    return -1; //the string got no end
}

int check_if_json_is_valid(char* JsonStartPtr){

}

int check_if_list_is_valid(char* ListStartPtr){

}

int check_json_validity(char* JsonArrPtr){
    if (JsonArrPtr == NULL){
        printf("pointer was null\n");
        return -1;
    }
    int json_level = 0;

    for (int i=0; i<strlen(JsonArrPtr); i++){
        //if main type is just a string
        if (JsonArrPtr[i] == '\"') {
            int result = check_if_list_is_valid(JsonArrPtr+i);
            if (result == -1){ //if invalid
                printf("String as main type was found. it was invalid.\n");
                return -1;
            }
            i += result;
        }
    }


    return 1;

    
}

int main(){
    char* current_str = \
    "{\
        \"id\":123, \
        \"username\":\"csaba001\", \
        \"friends\":[69, 420, 911], \
        \"userdata\":{ \
            \"is_admin\":true, \
            \"last_login\":\"2024-09-10@18:49:36\" \
        } \
    }";

    check_json_validity(current_str) == -1 ? printf("Json is invalid\n") : printf("Json is valid\n");

    return 0;
}