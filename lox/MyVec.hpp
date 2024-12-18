template <typename T>
class MyVec {
//private:
    
public:
    T* data;
    size_t cap;
    size_t size;
    
    MyVec(size_t cap){
        this->cap = cap;
        this->size = 0;
        this->data = (T*)malloc(sizeof(T)*cap);
        assert_ptr(this->data);
    }

    void add(T element){
        if (this->size >= this->cap){
            this->cap++;
            this->data = (T*)realloc((this->data), sizeof(T)*cap);
            assert_ptr(this->data);
            this->size++;
            memcpy(this->data+cap-1, &element, sizeof(T));
        }
        else {
            memcpy(this->data+size, &element, sizeof(T));
            this->size++;
        }
    }

    // ~MyVec(){ //destructor (called automatically)
    //     free(this->data);
    // }
};