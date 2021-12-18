/* hashmap.c */

#include "hashmap.h"

int gen_hash(HashMap* hashmap, const char* key) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 137 + (key[i] & 255)) % hashmap->size;
    }
    return hash;
}

void init_map(HashMap* hashmap, u_int32_t size) {
    hashmap->hash_table = (Data **) calloc(size, sizeof(Data *));
    for (int i = 0; i < size; i++) {
        hashmap->hash_table[i] = (Data *) malloc((size_t) sizeof(Data));
        hashmap->hash_table[i]->key[0] = '\0';
        hashmap->hash_table[i]->val[0] = '\0';
        hashmap->hash_table[i]->next = NULL;
    }
    hashmap->size = size;
    hashmap->amount = 0;
}

void store(HashMap* hashmap, const char* key, void *val) {
    int hash = gen_hash(hashmap, key);
    Data* data_node = hashmap->hash_table[hash];
    while (data_node->key[0] != '\0') {
        if (!strcmp(data_node->key, key)) { // update exist key's value
            strcpy(data_node->val, (Value *) val);
            return;
        }
        data_node = data_node->next;
    }
    strcpy(data_node->key, key);
    strcpy(data_node->val, (Value *) val);
    hashmap->amount++;
    data_node->next = (Data *) malloc((size_t) sizeof(Data));
    data_node->next->key[0] = '\0';
    data_node->next->val[0] = '\0';
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

void free_hashmap(HashMap *hashmap) {
    int i;
    Data *node = NULL;
    for (i = 0; i < hashmap->size; i++) {
        node = hashmap->hash_table[i];
        while(1) {
            Data *next = node->next;
            _FREE(node);
            if (next == NULL) {
                break;
            } else {
                node = next;
            }
        }
    }
    _FREE(hashmap->hash_table);
}
