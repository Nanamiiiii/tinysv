/* hashmap.h */

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 128
#define VAL_LENGTH 256

#ifndef __FREE__
#define __FREE__
#define _FREE(ptr) { \
    free(ptr); \
    ptr = NULL; \
} \

#endif

/*
 * hashmap for string
 */

typedef char Value;

typedef struct _data {
    char *key;
    Value *val;
    struct _data* next;
} Data;

typedef struct _hashmap {
    Data** hash_table;
    u_int32_t size;
    u_int32_t amount;
} HashMap;

int gen_hash(HashMap* hashmap, const char* key);
void init_map(HashMap* hashmap, u_int32_t size);
void store(HashMap* hashmap, const char* key, void* val);
void* get(HashMap* hashmap, const char* key);
void free_hashmap(HashMap *hashmap);

/*
 * hashmap for int
 */

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