typedef enum DataType {
    JS_NULL,
    NUMBER,
    BOOLEAN,
    STRING,
    LIST,
    JSON
} DataType;


struct JS_Object{
    //the variables are called current because references could be possible to lower values
    DataType Object_Dtype;
    char** Keys; //can be null (when it's not of JSON type), but can be an array of char pointers too

    unsigned int Value_Length; //in case of a list, represents the amount of Keys
    // JS_Object** Sub_Values; //an array of pointers, which have the type JS_Object (the last 8 continous 0 bytes stop the pointer list)
    
    void* Value_s; //the length variable describes it's length
    //it's an array -> it can hold more elements

};