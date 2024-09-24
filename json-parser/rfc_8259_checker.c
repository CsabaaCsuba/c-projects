#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

int check_json_validity(char* JsonArrPtr){
    if (JsonArrPtr == NULL){
        printf("pointer was null\n");
        return NULL;
    }

    
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

    check_json_validity(current_str == 1) ? printf("Json is valid\n") : printf("Json is invalid\n");

    return 0;
}