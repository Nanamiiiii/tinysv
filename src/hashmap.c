/* hashmap.c 
 */

#include "hashmap.h"

/*
 * hashmap for string
 */

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
        hashmap->hash_table[i]->key = (char *) calloc(KEY_LENGTH, sizeof(char));
        hashmap->hash_table[i]->val = (char *) calloc(VAL_LENGTH, sizeof(char));
        hashmap->hash_table[i]->next = NULL;
    }
    hashmap->size = size;
    hashmap->amount = 0;
}

void store(HashMap* hashmap, const char* key, void *val) {
    int hash = gen_hash(hashmap, key);
    Data* data_node = hashmap->hash_table[hash];
    while (data_node->next != NULL) {
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
    data_node->next->key = (char *) calloc(KEY_LENGTH, sizeof(char));
    data_node->next->val = (char *) calloc(VAL_LENGTH, sizeof(char));
    data_node->next->next = NULL;
}

void* get(HashMap* hashmap, const char* key) {
    int hash = gen_hash(hashmap, key);
    Data* data_node = hashmap->hash_table[hash];
    while (data_node->next != NULL) {
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
            _FREE(node->key);
            _FREE(node->val);
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

/*
 * hashmap for int
 */

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
        hashmap->hash_table[i]->next = NULL;
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
    data_node->next->next = NULL;
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

void free_hashmap_int(HashMap_int *hashmap) {
    int i;
    Data_int *node = NULL;
    for (i = 0; i < hashmap->size; i++) {
        node = hashmap->hash_table[i];
        while(1) {
            Data_int *next = node->next;
            free(node);
            if (next == NULL) {
                break;
            } else {
                node = next;
            }
        }
    }
    free(hashmap->hash_table);
}
