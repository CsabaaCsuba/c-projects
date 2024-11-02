#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Main conception: hash tables work by creating a hash of a string (usually)
Then the hash gets "modulod" with the hash table size, 
resulting in a number that is smaller than the length of the hash table.
This number will then be used as the index in the hash table

*/

#define bool unsigned char
#define true 1
#define false 0

struct ListNode {
    unsigned int key_len;
    unsigned int value_len;
    bool is_empty;
    char* key;
    char* value;

    struct ListNode* next;
};

struct Table {
    size_t table_size;
    struct ListNode* list;
};

size_t TABLE_SIZE = 4;

size_t hash(char* key){ //simple hashing function returning the idx of the 
    size_t hashnum = 0;
    size_t key_len = strlen(key);
    for (size_t i=0; i<key_len; i++){
        hashnum += key[i];
    }
    hashnum *= key_len;
    return hashnum;
}

unsigned int get_table_placement(char* key, size_t found_table_size){
    size_t hashnum = hash(key);
    return hashnum % found_table_size;
}


void init_node(struct ListNode* node){
    node->is_empty = true;
    node->key_len = 0;
    node->value_len = 0;
    node->key = NULL;
    node->value = NULL;
    node->next = NULL;
}

void init_linked_list(struct ListNode* linked_list, int table_row_count){
    for (int i=0; i<table_row_count; i++){
        init_node(linked_list+i);
    }
    printf("table initialized with the size of %d\n", table_row_count);
}

void init_table(struct Table* table, size_t input_len){
    struct ListNode* linked_list = malloc(input_len*sizeof(struct ListNode));
    init_linked_list(linked_list, input_len);
    table->list = linked_list;
    table->table_size = input_len;
}



void print_table(struct Table* table, int table_row_count){
    for (int i=0; i<table_row_count; i++){
        if (table->list[i].is_empty == true){
            continue; //just in case of big numbers
        }

        // printf("%d => %s\n", i, table[i].key);
        struct ListNode* current_node = table->list+i;
        printf("%d. ", i);
        while(true) { //this has to iterate to the end of the nodes
            printf("%s => %s", current_node->key, current_node->value);

            //for formatting purposes (a do while loop could have been used too)
            if (current_node->next == NULL){
                break;
            }
            else {
                current_node = current_node->next;
                printf(" | ");
            }
            
        };
        printf("\n");
    }
}

void set_key(struct Table* table, char* key, char* value){
    printf("key to set: %s\n", key);
    unsigned int table_idx = get_table_placement(key, table->table_size);
    // printf("table idx: %u\n", table_idx);
    struct ListNode* current_node = table->list+table_idx;
    // printf("current node selected\n");
    int i = 0;
    struct ListNode* filled_node = NULL;
    size_t key_len = strlen(key)+1;
    while(true) { //loop until the key has been found or the first empty node has been found
        printf("iteration %d\n", i);
        if (current_node->is_empty){ //if current node is empty, a filled_node can be selected
            printf("empty node found\n");
            filled_node = current_node;
            if (current_node->next == NULL){ //if the next node is unallocated, there is no need for allocation since this value can be filled
                break;
            }
        }
        else if (current_node->key != NULL){
            if (strncmp(current_node->key, key, key_len) == 0) { //if the keys are the same, the value has to be overwritten, this will be the filled node
                filled_node = current_node;
                printf("key already found between nodes\n");
                break;
            } 
        }
        if (current_node->next == NULL){ //if the next key would be null
            if (filled_node == NULL){ //if an empty node has not been found, allocate a new one which will be used
                printf("filled node is a node that is just created\n");
                current_node->next = malloc(sizeof(struct ListNode));
                init_node(current_node->next);
                filled_node = current_node->next;
                break;
            }
            else {
                break;
            }
            
        }
        current_node = current_node->next;

        i++;
    }
    printf("while loop done\n");//meow
    
    size_t value_len = strlen(value)+1;

    if (filled_node->key != NULL){ //if there is any previous key, free it's space
        free(filled_node->key);
    }
    filled_node->key = malloc(key_len);

    if (filled_node->value != NULL){ //if there is any previous value, free it's space
        free(filled_node->value);
    }
    filled_node->value = malloc(value_len);
    printf("malloc done\n");
    filled_node->key_len = key_len;
    filled_node->value_len = value_len;

    strncpy(filled_node->key, key, key_len-1);
    strncpy(filled_node->value, value, value_len-1);
    printf("copy done\n");
    filled_node->key[key_len-1] = '\0'; //make sure that there is a null terminator
    filled_node->value[value_len-1] = '\0';
    filled_node->is_empty = false;

}

void* clone(void* ptr, unsigned int length_in_bytes){
    if (ptr == NULL){
        printf("received ptr was null.\n");
        return NULL;
    }
    void* new_memspace = malloc(length_in_bytes);
    memcpy(new_memspace, ptr, length_in_bytes);
    return new_memspace;

}

char* get_from_table(struct Table* table, char* key){
    unsigned int idx = get_table_placement(key, table->table_size);
    struct ListNode* right_node = table->list+idx;
    while (right_node != NULL){
        if (right_node->key != NULL){ //if the current element of the node is not null
            if (strcmp(right_node->key, key) == 0){
                return clone(right_node->value, right_node->value_len); //return a copied value, so use-after-free can't happen
            }
        }
        right_node = right_node->next;

    }

    return NULL;
}

void remove_from_table(struct Table* table, char* key){
    //TODO: implement this so that the emptied node's key and value get NULLed, and the is_empty is set to true
    unsigned int idx = get_table_placement(key, table->table_size);
    struct ListNode* right_node = table->list+idx;
    while (right_node != NULL){
        if (right_node->key != NULL){ //if the current element of the node is not null
            if (strcmp(right_node->key, key) == 0){ //if the key is found
                free(right_node->key);
                free(right_node->value);
                right_node->key = NULL;
                right_node->value = NULL;
                right_node->is_empty = true;
                break;
            }
        }
        right_node = right_node->next;
    }
}

int main(){
    // char** linked_list = malloc(TABLE_SIZE*sizeof(char*));
    struct Table* table = malloc(TABLE_SIZE*sizeof(struct Table));

    init_table(table, TABLE_SIZE);

    set_key(table, "John", "Pork");

    set_key(table, "Kai", "Cenat");

    set_key(table, "apple", "banana");

    set_key(table, "a", "b");

    set_key(table, "halo", "szia");

    set_key(table, "bruh", "asd");

    // print_table(table, TABLE_SIZE);

    remove_from_table(table, "halo"); //removing halo to then replace it

    // printf("Value of John: %s\n", get_from_table(table, "John"));

    // set_key(table, "halo", "hi");

    // remove_from_table(table, "John");

    set_key(table, "hoho", "asd");

    set_key(table, "John", "Bro");

    printf("Value of halo: %s\n", get_from_table(table, "halo"));

    set_key(table, "a", "c");

    print_table(table, TABLE_SIZE);
    return 0;
}