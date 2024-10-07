#include <stdio.h>
#include <string.h>

int main(){
    // char* num = "-123.456";
    double my_double = -123.456000001;
    int bufsz = snprintf(NULL, 0, "%lf", my_double);
    printf("number's length: %d\n", bufsz);
    return 0;
}