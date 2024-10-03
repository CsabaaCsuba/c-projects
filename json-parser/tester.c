#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "json.h"

int main(){
    start_string_parsing("{\n\"name\":\"Alice\", \n\"age\":30, \t\"isStudent\":false,\n\r\"grades\":[85.5, 90, null], \"address\":{ \"city\":\"Wonderland\", \"zipcode\":12345 }, \
     \n \"Mother's name\":\"Nathalie Nathalieman\"}");

    start_string_parsing("2222222222");
    start_string_parsing("555555555555555.5");
    start_string_parsing("null");
    start_string_parsing("[null, \"anyad\"]");
    return 0;
}