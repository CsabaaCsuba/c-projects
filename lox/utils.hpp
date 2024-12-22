template <typename T>
class MyVec {
//private:
    
public:
    T* data;
    size_t cap;
    size_t size;
    
    MyVec(size_t cap){
        this->cap = cap * sizeof(T);
        this->size = 0;
        this->data = (T*)malloc(sizeof(T)*cap);
        assert_ptr(this->data);
    }

    void add(T element){
        if (this->size >= this->cap){
            this->cap += sizeof(T);
            this->data = (T*)realloc((this->data), sizeof(T)*cap);
            assert_ptr(this->data);
            this->size += sizeof(T);
            memcpy(this->data+cap-1, &element, sizeof(T));
        }
        else {
            memcpy(this->data+size, &element, sizeof(T));
            this->size += sizeof(T);
        }
    }

    T* begin(){
        return data;
    }

    T* end(){
        return data + size;
    }

    ~MyVec(){ //destructor (called automatically)
        free(this->data);
    }
};

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

void remove_whitespaces(char *str, size_t length) {
    if (str == NULL) {
        fprintf(stderr, "Error: Null pointer passed to remove_whitespaces.\n");
        return;
    }

    size_t i = 0, j = 0;

    // Iterate through the string
    while (i < length && str[i] != '\0') {
        // Check for whitespace characters manually
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
            str[j++] = str[i]; // Copy non-whitespace character
        }
        i++;
    }

    // Null-terminate the string if there's room
    if (j < length) {
        str[j] = '\0';
    } else {
        // If no room, terminate at the last safe position
        str[length - 1] = '\0';
    }
}