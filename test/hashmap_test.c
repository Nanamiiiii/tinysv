/* test/hashmap_test.c */
#include "hashmap.h"

int main(void) {
    HashMap hashmap;
    init_map(&hashmap, 32);
    char *test_val1 = "test value.";
    char *test_val2 = "test value 2.";
    store(&hashmap, "key1", test_val1);
    store(&hashmap, "key2", test_val2);
    char *get_1 = (char *) get(&hashmap, "key1");
    char *get_2 = (char *) get(&hashmap, "key2");
    if (strcmp(get_1, test_val1)) {
        printf("test_val1 is Incorrect.\n");
        return 1;
    }
    if (strcmp(get_2, test_val2)) {
        printf("test_val2 is Incorrect.\n");
        return 1;
    }
    printf("Test passed.\n");
    printf("get_1: %s\n", get_1);
    printf("get_2: %s\n", get_2);
    return 0;
}