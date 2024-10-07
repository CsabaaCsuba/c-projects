#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "json.h"

int main(){
    struct JS_Object* idk = start_string_parsing("{\
\"name\":\"Alice\", \
\"ageAAAAAAAAAAAAAAAAAAAAAaaa\":30, \
\"isStudent\":false, \
\"grades\":[85.5, 90, null, {\"semester\":\"Fall\", \"GPA\":3.8}], \
\"address\":{ \
    \"city\":\"Wonderland\", \
    \"zipcode\":12345, \
    \"coordinates\":{ \"latitude\":51.5074, \"longitude\":-0.1278 }, \
    \"aliases\": [\"Dreamland\", \"MysticCity\", \"The Great City\"] \
}, \
\"Mother's name\":\"Nathalie Nathalieman\", \
\"Father's name\":null, \
\"siblings\":[ \
    {\"name\":\"Bob\", \"age\":25, \"isStudent\":true, \"jobs\": [{\"company\":\"TechCorp\", \"position\":\"Engineer\"}]}, \
    {\"name\":\"Clara\", \"age\":28, \"isStudent\":false, \"hobbies\":[\"painting\", \"music\"]}, \
    null \
], \
\"isGraduated\":false, \
\"courses\":{ \
    \"current\":[\"Math\", \"Physics\", \"Computer Science\"], \
    \"completed\":[ \
        {\"name\":\"History\", \"year\":2019, \"grade\":\"A-\"}, \
        {\"name\":\"Biology\", \"year\":2020, \"grade\":\"B+\", \"specialNotes\":\"Lab work was excellent\"} \
    ], \
    \"upcoming\":[\"Philosophy\", \"Astronomy\"] \
}, \
\"preferences\":{ \
    \"colors\":[\"blue\", \"green\", \"red\"], \
    \"food\":{ \
        \"favorite\":\"Pizza\", \
        \"disliked\":\"Broccoli\" \
    }, \
    \"contact\":{ \
        \"email\":\"alice@example.com\", \
        \"phone\":null, \
        \"notifications\":true \
    } \
}, \
\"account_balance\":-200.75, \
\"lucky_numbers\":[7, 13, 22, 3.14], \
\"complex_number\":{ \
    \"real\":1.0, \"imaginary\":-1.0 \
}, \
\"escaped_characters\":\"Tab:\\t, Newline:\\n, Quote:\\\"\", \
\"nested_escapes\":\"Line Break\\r\\nAnother Line\" \
}");


    printf("\n\n\n%p\n", idk);

    // start_string_parsing("2222222222");
    // start_string_parsing("555555555555555.5");
    // start_string_parsing("null");
    // struct JS_Object* bruh = start_string_parsing("[null, \"anyad\"]");
    // printf("%p\n", bruh);
    return 0;
}