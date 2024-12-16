
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>

#ifndef errno_t
    #define errno_t uint64_t
#endif

errno_t fopen_s(FILE **f, const char *name, const char *mode) {
    errno_t ret = 0;
    assert(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}

class Lox {
public:
    static bool hadError;

    // Static initialization for hadError
    static void Initialize() {
        hadError = false;
    }

    static void RunFile(char* path) {
        printf("failed to read file\n");
        if (!path) {
            exit(-1);
        }
        char* buffer = nullptr;
        long length;
        FILE* f = nullptr;
        errno_t err = fopen_s(&f, path, "rb");

        if (err) {
            printf("reading the file failed.\n");
            hadError = true;
        }

        if (f) {
            fseek(f, 0, SEEK_END);
            length = ftell(f);
            fseek(f, 0, SEEK_SET);
            buffer = (char*)malloc(length);
            if (buffer) {
                fread(buffer, 1, length, f);
            }
            fclose(f);
        }

        if (buffer) {
            run(buffer);
        } else {
            printf("malloc failed\n");
            exit(-1);
        }

        free(buffer);
        if (hadError) {
            exit(65);
        }
    }

    static void runPrompt() {
        char buffer[1024] = "";
        while (true) {
            printf("> ");
            fgets(buffer, 1023, stdin);
            buffer[1023] = '\0';
            if (strncmp(buffer, "\n", 1) == 0) {
                break;
            }
            run(buffer);
            hadError = false;
        }
    }

    static void run(char* source) {
        printf("source: %s", source);
        // Add scanning and tokenizing logic here
    }

    static void error(int line, char* message) {
        report(line, (char*)"", message);
    }

    static void report(int line, char* where, char* message) {
        printf("[line %d] Error where %s: + %s", line, where, message);
        hadError = true;
    }
};

// Define the static variable
bool Lox::hadError = false;

// #include "TokenType.hpp"
#include "Scanner.hpp"

int main(int argc, char** argv) {
    Lox::Initialize(); // Initialize static data
    printf("arguments: [%d]\n", argc);
    for (int i = 1; i < argc; i++) {
        printf("[%d.] %s\n", i, argv[i]);
    }
    if (argc > 2) {
        printf("Usage: lox [script]");
        exit(64);
    } else if (argc == 2) {
        Lox::RunFile(argv[1]);
    } else {
        Lox::runPrompt(); // unimplemented
    }
}

