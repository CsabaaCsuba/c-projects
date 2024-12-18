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

        fprintf(file, "class %s {\n};", base_name);

        fclose(file);
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