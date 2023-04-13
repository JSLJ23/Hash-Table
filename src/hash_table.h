#ifndef HASH_TABLE

#define HASH_TABLE

#include <stdbool.h>

typedef struct Entry {
    char* key;
    char* value;
    struct Entry* next;
} Entry;


typedef struct HashTable {
    int size;
    int num_filled_slots;
    struct Entry** entries;
} HashTable;


HashTable* create_hash_table();
void hash_table_set_entry(HashTable* hashtable, const char* key, const char* value);
char* hash_table_get_entry(HashTable* hashtable, const char* key);
void hash_table_delete_entry(HashTable* hashtable, const char* key);
void print_hash_table(HashTable* hashtable);
void free_hash_table(HashTable* hashtable, bool verbose);

#endif