#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//doesn't require error handling
void shift_bytes(char* str, int amount_to_shift) {
    int len = strlen(str);
    if (amount_to_shift >= len) {
        return;
    }
    memmove(str, str + amount_to_shift, len - amount_to_shift + 1); // +1 for null terminator
}

int main() {
    // Create a dynamically allocated string
    char *str = malloc(12);  // 11 characters + 1 for null terminator
    strcpy(str, "halloMyName");

    printf("Original string: %s\n", str);

    // Shift the first 3 bytes
    shift_bytes(str, 7);

    printf("Shifted string: %s\n", str);

    // Free allocated memory
    free(str);

    return 0;
}