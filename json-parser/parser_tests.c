#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"

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

    read_raw_string(current_str);

    return 0;
}