/* hashmap_int.h */

#ifndef __HASHMAP_INT_H__
#define __HASHMAP_INT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 128

typedef int Value_int;

typedef struct _data_int {
    char key[KEY_LENGTH];
    Value_int val;
    struct _data_int* next;
} Data_int;

typedef struct _hashmap_int {
    Data_int** hash_table;
    u_int32_t size;
    u_int32_t amount;
} HashMap_int;

int gen_hash_int(HashMap_int* hashmap, const char* key);
void init_map_int(HashMap_int* hashmap, u_int32_t size);
void store_int(HashMap_int* hashmap, const char* key, Value_int val);
Value_int *get_int(HashMap_int* hashmap, const char* key);
void free_hashmap_int(HashMap_int *hashmap);

#endif