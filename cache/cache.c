#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap_hashtable.h"

#define KEYWORD_BUFSIZ 2048

void ascii_convert_to_uppercase(char* base_string){
    for (size_t i=0; i<strlen(base_string); i++){
        if (base_string[i] >= 97 || base_string[i] <= 122){
            base_string[i] -= 32;
        }
    }
}

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


void parse_command(char command[BUFSIZ], char command_store[3][KEYWORD_BUFSIZ]){
    char keyword_storage[KEYWORD_BUFSIZ];
    int r = 0;
    int i = 0;
    while (i < 3){
        r = get_next_keyword(r, command, keyword_storage);
        // printf("keyword was: %s\nresult: %d\n", keyword_storage, r);
        if (r == -1){
            break;
        }
        strncpy(command_store[i], keyword_storage, KEYWORD_BUFSIZ-1);
        command_store[i][KEYWORD_BUFSIZ-1] = '\0';
        i++;
    }


}

void read_and_exec(struct Table* table){
    char buffer[BUFSIZ];

    char command_storage[3][KEYWORD_BUFSIZ];
    for (int i=0; i<3; i++){
        command_storage[i][0] = '\0';
    }

    printf("cache>");
    if (fgets(buffer, BUFSIZ, stdin) != NULL){
        // printf("read command: %s", buffer);
        size_t command_len = strlen(buffer);
        if (command_len < 7){
            printf("command too short.\n");
            return;
        }
        // printf("\"%c\"", );
        buffer[command_len-1] = '\0'; //remove the newline character
        if (buffer[command_len-2] == '\r'){ //replace return carriage on windows
            buffer[command_len-2] = '\0';
        }
        parse_command(buffer, command_storage);
    }
    ascii_convert_to_uppercase(command_storage[0]);
    // strcpy(command_storage[0], ); //made the first uppercase

    if (strncmp(command_storage[0], "SET", 4) == 0){
        if (command_storage[1][0] == '\0' || command_storage[2][0] == '\0'){
            printf("other strings are empty. cannot execute.\n");
        }
        else {
            // printf("set command valid\n");
            set_key(table, command_storage[1], command_storage[2]);
            printf("\"OK\"\n");
        }
    }

    if (strncmp(command_storage[0], "GET", 4) == 0){
        if (command_storage[1][0] == '\0'){
            printf("command too short: Usage: GET key\n");
        }
        else if (command_storage[2][0] != '\0'){
            printf("command too long. Usage: GET key\n");
        }
        else {
            printf("%s\n", get_from_table(table, command_storage[1]));
        }
    }

    if (strncmp(command_storage[0], "DEL", 4) == 0){
        if (command_storage[1][0] == '\0'){
            printf("command too short: Usage: DEL key\n");
        }
        else if (command_storage[2][0] != '\0'){
            printf("command too long. Usage: DEL key\n");
        }
        else {
            remove_from_table(table, command_storage[1]);
            printf("ok\n");
        }
    }

    if (strncmp(command_storage[0], "GETALL", 7) == 0){
        print_table(table);
    }




    //TODO: check for syntax: SET key_name value_name GET/DEL key_name GETALL
    //for now, nothing else is acceptable
}

const size_t table_size = 4096;

int main(){
    // char** linked_list = malloc(TABLE_SIZE*sizeof(char*));
    struct Table* table = malloc(table_size*sizeof(struct Table));

    init_table(table, table_size);

    while (true){
        read_and_exec(table);
    }

    return 0;
}