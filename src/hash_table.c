#include "hash_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BLU   "\x1B[34m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"


#define TABLE_SIZE 100000


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


// Create the hash table.
HashTable* create_hash_table()
{
    // Allocate memory for the HashTable struct.
    HashTable* hashtable = malloc(sizeof(HashTable));

    // Set the number of filled slots to zero;
    hashtable->num_filled_slots = 0;

    // Set the table size based on the globally defined table size.
    hashtable->size = TABLE_SIZE;

    // Allocate memory for the array of entries (size of 1 entry × number of entries).
    // Entries is an array of pointers to individual entries.
    hashtable->entries = malloc(hashtable->size * sizeof(Entry));

    // Set each entry to NULL.
    for (int i = 0; i < hashtable->size; i++) {
        hashtable->entries[i] = NULL;
    }

    // Return the pointer to the hashtable.
    return hashtable;
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


// Add an entry (with key value pairs).
// The entry is created internally in this function so the user doesn't need to call the
// create_entry() function explicitly.
void hash_table_set_entry(HashTable* hashtable, const char* key, const char* value)
{
    // Create an entry
    Entry* entry = create_entry(key, value);

    // Get the hash of that entry's key
    unsigned int hash_value = hash_function(key);

    // With that hash, get the current contents of that slot in the hash table.
    Entry* current_entry = hashtable->entries[hash_value];

    // If that slot is not taken, i.e. the entry there is NULL, insert this current entry there.
    if (current_entry == NULL) {
        hashtable->entries[hash_value] = entry;
        hashtable->num_filled_slots++;
        return;
    }

    // If there is already an entry there, we need to check if the string values of the keys are a
    // match, if so we need to override that value. If not, we add the new entry to the end of the
    // link list.
    Entry* previous;
    while (current_entry != NULL) {
        // If the key already exist, we overwrite the value.
        if (strcmp(current_entry->key, key) == 0) {
            free(current_entry->value);
            current_entry->value = malloc(strlen(value) + 1); // +1 for the null terminator.
            strcpy(current_entry->value, value);
            return;
        }
        // If not we advance to the next item in the linked list and check the key there.
        previous      = current_entry;
        current_entry = previous->next;
    }
    // If there are no matches, we add the new entry to the end of the linked list (right before
    // current_entry == NULL).
    previous->next = entry;
    hashtable->num_filled_slots++;
}


// Lookup an entry.
char* hash_table_get_entry(HashTable* hashtable, const char* key)
{
    // Get the hash value for this key.
    unsigned int hash_value = hash_function(key);

    // Get the entry at that hash value.
    Entry* entry = hashtable->entries[hash_value];

    // If there is no entry at that slot, return NULL.
    if (entry == NULL)
        return NULL;

    // If there is an entry at that slot, check the key to see if it matches and do so for all
    // entries in the linked list. If none of the keys match, return NULL.
    while (entry != NULL) {
        // Check if the keys match.
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        // Proceed to the next entry of the linked list.
        entry = entry->next;
    }

    return NULL;
}


// Deleting an entry from the hash table.
// This function is slightly complex has it has to deal with multiple scenarios but I will try to
// add as much comments to make it more understandable.
void hash_table_delete_entry(HashTable* hashtable, const char* key)
{
    // Get the hash value for this key.
    unsigned int hash_value = hash_function(key);

    // Get the entry at that hash value.
    Entry* entry = hashtable->entries[hash_value];

    // If there is no entry at that slot, return and don't do anything.
    if (entry == NULL)
        return;

    // If there is an entry at that slot, check the key to see if it matches and do so for all
    // entries in the linked list.
    // We need a temporary pointer to keep track of the previous entry as we progress through this
    // list.
    Entry* previous;
    while (entry != NULL) {
        // String compare the keys for a lexicographical match.
        if (strcmp(entry->key, key) == 0) {
            // There can be 4 main scenarios for trying to delete an entry given an exact key match:

            // 1. The entry is the only entry in the linked list.
            // 2. The entry is the first entry in the linked list with next entries.
            // 3. The entry is the last entry in the linked list.
            // 4. The entry is somewhere in the middle of the linked list.

            // Scenario 1: The entry is the only entry in the linked list.
            // If this is so, we can just set that entry to NULL and free the memory for this entry.
            if (previous == NULL && entry->next == NULL) {
                hashtable->entries[hash_value] = NULL;
            }

            // Scenario 2: The entry is the first entry in the linked list with other entries that
            // follow. If this is so, we set the next entry to the current slot for that hash value
            // and free the memory for this entry.
            else if (previous == NULL && entry->next != NULL) {
                hashtable->entries[hash_value] = entry->next;
            }

            // Scenario 3: The entry is the last entry of the linked list.
            // If this is so, we set the ->next of the previous entry to null and free the memory
            // for this entry.
            else if (previous != NULL && entry->next == NULL) {
                previous->next = NULL;
            }

            // Scenario 4: The entry is somewhere in the middle of the linked list.
            // If this is so, we need to move the current entry's next pointer to the previous
            // entry's next pointer and essentially skip this current entry, then we free the memory
            // for this entry.
            else if (previous != NULL && entry->next != NULL) {
                previous->next = entry->next;
            }

            // Free memory for the entry and exit the function.
            free(entry->key);
            free(entry->value);
            free(entry);
            hashtable->num_filled_slots--;
            return;
        }

        // Advance the pointers.
        // Previous is now set to the current entry and the current entry advances to it's next.
        previous = entry;
        entry    = previous->next;
    }
}


// Display all entries.
void print_hash_table(HashTable* hashtable)
{
    // Iterate through each entry in the table.
    for (int i = 0; i < hashtable->size; i++) {
        Entry* entry = hashtable->entries[i];

        // If there is no entry, just skip and do nothing.
        if (entry == NULL)
            continue;

        // If there is an entry at that slot, iterate through all entries on the linked list and
        // print each entry's key and value. Print the slot (i.e. the hash value).
        printf(BLU "Slot[%d]:\n" RESET, i);
        while (entry != NULL) {
            printf("Key: %s, Value: %s\n", entry->key, entry->value);
            entry = entry->next;
        }
    }
}


// Free memory of the entire hash table.
void free_hash_table(HashTable* hashtable, bool verbose)
{
    // Iterate through each entry in the table.
    for (int i = 0; i < hashtable->size; i++) {
        Entry* entry = hashtable->entries[i];

        // If there is no entry, just skip and do nothing.
        if (entry == NULL)
            continue;

        // If there is an entry at that slot, iterate through all entries of the linked list and
        // free memory one by one.
        Entry* previous;
        while (entry != NULL) {
            // Pointer increment.
            previous = entry;
            entry    = previous->next;
            // Printing out the entries we have free memory for.
            if (verbose) {
                printf(RED
                       "Freeing memory at slot[%d] for key: %s, value: %s at address %p.\n" RESET,
                       i, previous->key, previous->value, previous);
            }
            free(previous->key);
            free(previous->value);
            free(previous);
        }
    }

    // Free the memory for the entries array of the hashtable and the hashtable struct itself.
    free(hashtable->entries);
    free(hashtable);
}
