void assert_ptr(void* ptr){
    if (ptr == (void*)0){
        printf("assertion for pointer failed.\n");
    }
}

char* create_substring(char* main_ptr, size_t start_idx, size_t end_idx){
    size_t substring_size = end_idx - start_idx;
    char* substring = (char*)malloc(substring_size);
    assert_ptr(substring);
    memcpy(substring, main_ptr+start_idx, substring_size);

    substring[substring_size-1] = '\0';
    return substring;
}