
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "TokenType.hpp"



class Lox {
    

public:
    bool hadError;
    Lox() {
        hadError = false;
    };

    void RunFile(char* path) {
        printf("failed to read file\n");
        if (!path) {
            exit(-1);
        }
        char* buffer = 0;
        long length;
        FILE* f = nullptr;
        errno_t err = fopen_s(&f, path, "rb");

        if (f)
        {
            fseek(f, 0, SEEK_END);
            length = ftell(f);
            fseek(f, 0, SEEK_SET);
            buffer = (char*)malloc(length);
            if (buffer)
            {
                fread(buffer, 1, length, f);
            }
            fclose(f);
        }

        if (buffer)
        {
            run(buffer);
        }
        else {
            printf("malloc failed\n");
            exit(-1);
        }

        free(buffer);
        if (this->hadError) {
            exit(65);
        }
    }

    void runPrompt() {
        char buffer[1024] = "";
        while (true) {
            printf("> ");
            fgets(buffer, 1023, stdin);
            buffer[1023] = '\0';
            //printf("read input: %s", buffer);
            if (strncmp(buffer, "\n", 1) == 0) {
                break;
            }
            run(buffer);
            this->hadError = false;
        }



    }

    static void run(char* source) {
        //Scanner scanner = new Scanner(source);
        //Token* tokens = scanner.scanTokens();

        //foreach(Token token : tokens) {
        //    printf("%d", token);
        //}
    }

    void error(int line, char* message) {
        this->report(line, (char*)"", message);
    }
    void report(int line, char* where, char* message) {
        printf("[line %d] Error where %s: + %s", line, where, message);
        this->hadError = true;
    }
};

int main(int argc, char** argv)
{
    Lox interpreter = Lox();
    printf("arguments: [%d]\n", argc);
    for (int i = 1; i < argc; i++) {
        printf("[%d.] %s\n", i, *argv);
    }
    if (argc > 2) {
        printf("Usage: lox [script]");
        exit(64);
    }
    else if (argc == 2) {
        
        interpreter.RunFile(argv[1]);
    }
    else {
        interpreter.runPrompt(); // unimplemented
    }
}


