#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include "MyVec.hpp"
#include "utils.hpp"


class GenerateAst {
public:
    static void defineAst(char* output_dir, char* base_name, MyVec<char*> types){
        size_t file_name_len = snprintf(nullptr, 0, "%s/%s.cpp", output_dir, base_name) + 1;
        char* file_name = (char*)malloc(file_name_len);
        assert_ptr(file_name);
        
        snprintf(file_name, file_name_len, "%s/%s.cpp", output_dir, base_name);

        FILE* file = fopen(file_name, "w");
        if (file == NULL) {
            perror("Error creating file\n");
        }

        fprintf(file, "#include \"utils.hpp\"\nclass %s {\n", base_name);

        for (char* type : types){
            char* colon_pos = strchr(type, ':');
            defineTypeLowLevel(file, base_name, type, colon_pos);


        }

        fprintf(file, "};");
        fclose(file);
    }

private:
    static void defineTypeLowLevel(FILE* fileptr, char* base_name, char* str_start, char* colon_pos){
        *colon_pos = '\0'; //by replacing the colon with a null character, the fprintf function can be tricked
        //str_start is only "read" until the end of the classname
        fprintf(fileptr, "class %s : %s {\n", str_start, base_name);
        
        //constructor
        fprintf(fileptr, "\t%s(%s){", str_start, colon_pos+1);
        

    }



};

int main(int argc, char** argv) {
    if (argc != 2){
        printf("usage: generate_ast <output directory>");
    }
    char* output_dir = argv[1];
    MyVec<char*> arrays(10);
    arrays.add("Binary   : Expr left, Token operator, Expr right");
    arrays.add("Grouping : Expr expression");
    arrays.add("Literal  : Object value");
    arrays.add("Unary    : Token operator, Expr right");
    GenerateAst::defineAst(output_dir, "Expr", arrays);

    return 0;
}