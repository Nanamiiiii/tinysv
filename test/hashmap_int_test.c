/* test/hashmap_int_test.c */

#include "hashmap_int.h"

int main(void) {
    HashMap hashmap;
    init_map_int(&hashmap, 32);
    int test_val1 = 100;
    int test_val2 = 114514;
    store_int(&hashmap, "key1", test_val1);
    store_int(&hashmap, "key2", test_val2);
    int *get_1 = (int *) get_int(&hashmap, "key1");
    int *get_2 = (int *) get_int(&hashmap, "key2");
    if (*get_1 != test_val1) {
        printf("test_val1 is Incorrect.\n");
        return 1;
    }
    if (*get_2 != test_val2) {
        printf("test_val2 is Incorrect.\n");
        return 1;
    }
    printf("Test passed.\n");
    printf("get_1: %d\n", *get_1);
    printf("get_2: %d\n", *get_2);
    return 0;
}