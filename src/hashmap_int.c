/* hashmap_int.c */

#include "hashmap_int.h"

int gen_hash_int(HashMap_int* hashmap, const char* key) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 137 + (key[i] & 255)) % hashmap->size;
    }
    return hash;
}

void init_map_int(HashMap_int* hashmap, u_int32_t size) {
    hashmap->hash_table = (Data_int**) calloc(size, sizeof(Data_int*));
    for (int i = 0; i < size; i++) {
        hashmap->hash_table[i] = (Data_int*) malloc((size_t) sizeof(Data_int));
        hashmap->hash_table[i]->key[0] = '\0';
    }
    hashmap->size = size;
    hashmap->amount = 0;
}

void store_int(HashMap_int* hashmap, const char* key, Value_int val) {
    int hash = gen_hash_int(hashmap, key);
    Data_int* data_node = hashmap->hash_table[hash];
    while (data_node->key[0] != '\0') {
        if (!strcmp(data_node->key, key)) { // update exist key's value
            data_node->val = val;
            return;
        }
        data_node = data_node->next;
    }
    strcpy(data_node->key, key);
    data_node->val = val;
    hashmap->amount++;
    data_node->next = (Data_int*) malloc((size_t) sizeof(Data_int));
    data_node->next->key[0] = '\0';
}

Value_int *get_int(HashMap_int* hashmap, const char* key) {
    int hash = gen_hash_int(hashmap, key);
    Data_int* data_node = hashmap->hash_table[hash];
    while (data_node->key[0] != '\0') {
        if (!strcmp(data_node->key, key)) {
            return &data_node->val;
        }
        data_node = data_node->next;
    }
    return NULL;
}
