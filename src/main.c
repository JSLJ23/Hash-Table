#include "hash_table.h"
#include <stdbool.h>




// Main function.
int main(int argc, char* argv[])
{
    HashTable* my_hash_table = create_hash_table();

    hash_table_set_entry(my_hash_table, "food1", "hamburgers");
    hash_table_set_entry(my_hash_table, "food2", "noodles");
    hash_table_set_entry(my_hash_table, "food3", "pizza");
    hash_table_set_entry(my_hash_table, "food4", "fried rice");
    hash_table_set_entry(my_hash_table, "food5", "porridge");
    hash_table_set_entry(my_hash_table, "food6", "fish n chips");
    hash_table_set_entry(my_hash_table, "food7", "steak");

    print_hash_table(my_hash_table);

    free_hash_table(my_hash_table, true);

    return 0;
}