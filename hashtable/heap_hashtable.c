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

typedef struct ListNode {
    unsigned int key_len;
    unsigned int value_len;
    bool is_empty;
    char* key;
    char* value;

    struct ListNode* next;
} ListNode;

int TABLE_SIZE = 4;
int BUFF_SIZE = 2048;
int DEFAULT_SIZE = 128;

unsigned int hash(char* key){ //simple hashing function returning the idx of the 
    unsigned int hashnum = 0;
    size_t key_len = strlen(key);
    for (size_t i=0; i<key_len; i++){
        hashnum += key[i];
    }
    hashnum *= key_len;
    return hashnum;
}

unsigned int get_table_placement(unsigned int hash){
    return hash % TABLE_SIZE;
}

void init_node(struct ListNode* node){
    node->is_empty = true;
    node->key_len = 0;
    node->value_len = 0;
    node->key = NULL;
    node->value = NULL;
}

void init_table(struct ListNode* hashtable, int table_row_count){
    for (int i=0; i<table_row_count; i++){
        init_node(hashtable+i);
    }
    printf("table initialized with the size of %d\n", table_row_count);
}



void print_table(struct ListNode* hashtable, int table_row_count){
    for (int i=0; i<table_row_count; i++){
        // printf("%d => %s\n", i, hashtable[i].key);
        struct ListNode* current_node = hashtable+i;
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

void add_to_table(struct ListNode* table, char* key, char* value){
    printf("bruh\n");
    unsigned int table_idx = get_table_placement(hash(key));
    printf("table idx: %d\n", table_idx);
    struct ListNode* current_node = table+table_idx;
    printf("current node selected\n");
    while (current_node->is_empty != true){ //go through the "bucket" until the first unused "slot" has been found
        if (current_node->next == NULL){
            current_node->next = malloc(sizeof(struct ListNode));
            init_node(current_node->next);
        }
        current_node = current_node->next; //on the next iteration, it's gona check the current_node anyway
    }
    printf("while loop done\n");
    size_t key_len = strlen(key)+1;
    size_t value_len = strlen(value)+1;

    if (current_node->key != NULL){ //if there is any previous key, free it's space
        free(current_node->key);
    }
    current_node->key = malloc(key_len);

    if (current_node->value != NULL){ //if there is any previous value, free it's space
        free(current_node->value);
    }
    current_node->value = malloc(value_len);
    printf("malloc done\n");
    current_node->key_len = key_len;
    current_node->value_len = value_len;

    strncpy(current_node->key, key, key_len-1);
    strncpy(current_node->value, value, value_len-1);
    printf("copy done\n");
    current_node->key[key_len-1] = '\0'; //make sure that there is a null terminator
    current_node->value[value_len-1] = '\0';
    current_node->is_empty = false;

}

void* clone(void* ptr, unsigned int length_in_bytes){
    if (ptr == NULL){
        printf("received ptr was null.\n");
        return NULL;
    }
    void* new_memspace = calloc(length_in_bytes, 1);
    memcpy(new_memspace, ptr, length_in_bytes);
    return new_memspace;

}

char* get_from_table(struct ListNode* table, char* key){
    unsigned int idx = get_table_placement(hash(key));
    struct ListNode* right_node = table+idx;
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

void remove_from_table(struct ListNode* table, char* key){
    //TODO: implement this so that the emptied node's key and value get NULLed, and the is_empty is set to true
    unsigned int idx = get_table_placement(hash(key));
    struct ListNode* right_node = table+idx;
    while (right_node != NULL){
        if (right_node->key != NULL){ //if the current element of the node is not null
            if (strcmp(right_node->key, key) == 0){ //if the key is found
                free(right_node->key);
                free(right_node->value);
                right_node->key = NULL;
                right_node->value = NULL;
                break;
            }
        }
        right_node = right_node->next;
    }

}

int main(){
    // char** hashtable = malloc(TABLE_SIZE*sizeof(char*));
    struct ListNode* hashtable = malloc(TABLE_SIZE*sizeof(ListNode));

    init_table(hashtable, TABLE_SIZE);

    add_to_table(hashtable, "John", "Pork");

    add_to_table(hashtable, "Kai", "Cenat");

    add_to_table(hashtable, "apple", "banana");

    add_to_table(hashtable, "a", "b");

    add_to_table(hashtable, "halo", "szia");

    print_table(hashtable, TABLE_SIZE);

    remove_from_table(hashtable, "halo");

    printf("Value of John: %s\n", get_from_table(hashtable, "John"));

    remove_from_table(hashtable, "John");

    printf("Value of halo: %s\n", get_from_table(hashtable, "halo"));

    print_table(hashtable, TABLE_SIZE);
    return 0;
}