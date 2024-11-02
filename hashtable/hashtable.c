#include <stdio.h>
#include <string.h>

/*
Main conception: hash tables work by creating a hash of a string (usually)
Then the hash gets "modulod" with the hash table size, 
resulting in a number that is smaller than the length of the hash table.
This number will then be used as the index in the hash table

Weakness: this can't do anything in case of hash collisions

*/


const unsigned int TABLE_SIZE = 32;
const unsigned int BUFF_SIZE = 2048;

unsigned int hash(char* key){ //simple hashing function returning the idx of the 
    unsigned int hashnum = 0;
    size_t key_len = strlen(key);
    for (size_t i=0; i<key_len; i++){
        hashnum += key[i];
    }
    hashnum *= key_len;
    return hashnum;
}

unsigned int get_table_placement(char* key){
    unsigned int hash_value = hash(key);
    return hash_value % TABLE_SIZE;
}

void init_table(char hashtable[TABLE_SIZE][BUFF_SIZE], int table_row_count){
    for (int i=0; i<table_row_count; i++){
        hashtable[i][0] = '\0';
    }
    printf("table initialized with the size of %d\n", table_row_count);
}

void print_table(char hashtable[TABLE_SIZE][BUFF_SIZE], int table_row_count){
    for (int i=0; i<table_row_count; i++){
        printf("%d => %s\n", i, hashtable[i]);
    }
}

void add_to_table(char table[TABLE_SIZE][BUFF_SIZE], char* key, char* value){
    unsigned int table_idx = get_table_placement(key);
    strncpy(table[table_idx], value, BUFF_SIZE-1);
    table[table_idx][BUFF_SIZE-1] = '\0'; //so string ends at the end of BUFF_SIZE
}

char* get_from_table(char table[TABLE_SIZE][BUFF_SIZE], char* key){
    unsigned int idx = get_table_placement(key);
    return table[idx];
}

void remove_from_table(char table[TABLE_SIZE][BUFF_SIZE], char* key){
    unsigned int idx = get_table_placement(key);
    if (table[idx][0] != '\0'){ //if it's not zero, set it back to zero
        table[idx][0] = '\0';
    }
}

int main(){
    char hashtable[TABLE_SIZE][BUFF_SIZE];

    init_table(hashtable, TABLE_SIZE);

    add_to_table(hashtable, "John", "Pork");

    add_to_table(hashtable, "Kai", "Cenat");

    add_to_table(hashtable, "apple", "banana");

    add_to_table(hashtable, "a", "b");

    print_table(hashtable, TABLE_SIZE);

    printf("Value of John: %s\n", get_from_table(hashtable, "John"));
    return 0;
}