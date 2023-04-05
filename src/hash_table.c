#include "hash_table.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define RED   "\x1B[31m"
#define RESET "\x1B[0m"


#define TABLE_SIZE 100000


typedef struct Entry {
    char* key;
    char* value;
    struct Entry* next;
} Entry;


typedef struct HashTable {
    int size;
    struct Entries** entries;
} HashTable;


// Hash function.
unsigned int hash_function(const char* key)
{

    unsigned int hash_value = 0;
    unsigned int key_length = strlen(key);

    for (int i = 0; i < key_length; i++) {
        hash_value += hash_value + key[i] % 37;
    }

    hash_value %= TABLE_SIZE;

    return hash_value;
}


// Creating an entry.
Entry* create_entry(const char* key, const char* value)
{
    // Allocate memory for this entry.
    Entry* entry = malloc(sizeof(Entry));
    entry->key   = malloc(strlen(key) + 1);   // +1 for null terminator (\0).
    entry->value = malloc(strlen(value) + 1); // +1 for null terminator (\0).

    // Copy the key and value strings into the memory allocated on the Entry Struct.
    strcpy(entry->key, key);
    strcpy(entry->value, value);

    // Set the next pointer to NULL.
    entry->next = NULL;

    return entry;
}


// Add an entry.


// Lookup an entry.


// Deleting an entry.


// Display all entries.


// Free memory of the entire hash table.
