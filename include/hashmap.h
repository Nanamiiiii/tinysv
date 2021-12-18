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
    printf("%p\n", ptr); \
    free(ptr); \
    ptr = NULL; \
} \

#endif

typedef char Value;

typedef struct _data {
    char key[KEY_LENGTH];
    Value val[VAL_LENGTH];
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

#endif