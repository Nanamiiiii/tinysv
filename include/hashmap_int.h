/* hashmap_int.h */

#ifndef __HASHMAP_INT_H__
#define __HASHMAP_INT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 128

typedef int Value;

typedef struct _data {
    char key[KEY_LENGTH];
    Value val;
    struct _data* next;
} Data;

typedef struct _hashmap {
    Data** hash_table;
    u_int32_t size;
    u_int32_t amount;
} HashMap;

int gen_hash_int(HashMap* hashmap, const char* key);
void init_map_int(HashMap* hashmap, u_int32_t size);
void store_int(HashMap* hashmap, const char* key, Value val);
Value *get_int(HashMap* hashmap, const char* key);

#endif