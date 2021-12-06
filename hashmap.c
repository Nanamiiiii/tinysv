#include "hashmap.h"

int gen_hash(HashMap* hashmap, const char* key) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 137 + (key[i] & 255)) % hashmap->size;
    }
    return hash;
}

void init_map(HashMap* hashmap, u_int32_t size) {
    hashmap->hash_table = (Data**) calloc(size, sizeof(Data*));
    for (int i = 0; i < size; i++) {
        hashmap->hash_table[i] = (Data*) malloc((size_t) sizeof(Data));
    }
    hashmap->size = size;
    hashmap->amount = 0;
}

void store(HashMap* hashmap, const char* key, void* val) {
    int hash = gen_hash(hashmap, key);
    Data* data_node = hashmap->hash_table[hash];
    while (data_node->key[0] != '\0') {
        if (!strcmp(data_node->key, key)) { // update exist key's value
            data_node->val = *((Value*) val);
            return;
        }
        data_node = data_node->next;
    }
    strcpy(data_node->key, key);
    data_node->val = *((Value*) val);
    hashmap->amount++;
    data_node->next = (Data*) malloc((size_t) sizeof(Data));
}

void* get(HashMap* hashmap, const char* key) {
    int hash = gen_hash(hashmap, key);
    Data* data_node = hashmap->hash_table[hash];
    while (data_node->key[0] != '\0') {
        if (!strcmp(data_node->key, key)) {
            return data_node->val;
        }
        data_node = data_node->next;
    }
    return NULL;
}
