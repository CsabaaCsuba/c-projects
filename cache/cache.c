#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap_hashtable.h"

#define KEYWORD_BUFSIZ 2048

int get_next_keyword(int idx, char command[BUFSIZ], char keyword_addr[KEYWORD_BUFSIZ]){
    char current_keyword[KEYWORD_BUFSIZ] = "";
    int i = 0;
    bool is_in_string = false;
    bool legit_encountered = false; //if legit char is encountered or not
    for (; i<KEYWORD_BUFSIZ && command[idx] != '\0'; idx++){
        // printf("i: %d, idx: %d\n", i, idx);
        if (command[idx] == ' ' || command[idx] == '\t'){
            if (!legit_encountered){
                // printf("ignoring space\n");
                legit_encountered = false;
                continue;
            }
            else if (is_in_string){
                current_keyword[i] = command[idx];
                i++;
            }
            else {
                break;
            }
            
        }
        else if (command[idx] == '\"'){
            if (is_in_string){
                is_in_string = false;
            }
            else {
                is_in_string = true;
                legit_encountered = true;
            }
        }
        else {
            if (!legit_encountered){
                legit_encountered = true;
            }
            if (legit_encountered && i < KEYWORD_BUFSIZ - 1){
                current_keyword[i] = command[idx];
                i++;
            }
        }
    }
    current_keyword[i] = '\0';

    strncpy(keyword_addr, current_keyword, KEYWORD_BUFSIZ);

    if (strlen(current_keyword) == 0){
        return -1; //failed to read
    }

    return idx+1; //r incremented because of the space character
}


int parse_command(char command[BUFSIZ], char command_store[3][KEYWORD_BUFSIZ]){
    char keyword_storage[KEYWORD_BUFSIZ];
    // int i = 0;
    int r = 0;
    int i = 0;
    while (i < 3){
        r = get_next_keyword(r, command, keyword_storage);
        printf("keyword was: %s\nresult: %d\n", keyword_storage, r);
        if (r == -1){
            return r;
        }
        strncpy(command_store[i], keyword_storage, KEYWORD_BUFSIZ-1);
        command_store[i][KEYWORD_BUFSIZ-1] = '\0';
        i++;
    }

    return 0; //for success

}

void read_and_exec(struct Table* table){
    char buffer[BUFSIZ];

    char command_storage[3][KEYWORD_BUFSIZ];

    printf("cache>");
    if (fgets(buffer, BUFSIZ, stdin) != NULL){
        printf("read command: %s", buffer);
        // printf("\"%c\"", );
        buffer[strlen(buffer)-1] = '\0';
        if (parse_command(buffer, command_storage) == -1){
            printf("execution stopped. error encountered.\n");
            return;
        }
    }
    for (int i = 0; i<3; i++){
        printf("%s\n", command_storage[i]);
    }

    //TODO: check for syntax: SET/GET/REMOVE key_name value_name
    //for now, nothing else is acceptable

    printf("%p\n", table);
}

int main(){
    // char** linked_list = malloc(TABLE_SIZE*sizeof(char*));
    struct Table* table = malloc(TABLE_SIZE*sizeof(struct Table));

    init_table(table, TABLE_SIZE);

    while (true){
        read_and_exec(table);
    }

    return 0;
}