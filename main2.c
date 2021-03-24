
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

// This is a comparison function that treats keys as signed ints
int compare_int_keys(void *key1, void *key2)
{
    // Cast them as int* and read them in
    int key1_v = *((int *) key1);
    int key2_v = *((int *) key2);

    // Perform the comparison
    if (key1_v < key2_v) {
        return -1;

    } else if (key1_v == key2_v) {
        return 0;

    } else {
        return 1;
    }
}


int main(int argc, char **argv)
{
    int i;
    heap h;
    int count = 10000;
    heap_cmp_func cmp = compare_int_keys;

    heap_create(&h, count, cmp);

    // Allocate a key and value
    int *key = (int *) malloc(count * sizeof(int));
    int *key2 = (int *) malloc(count * sizeof(int));

    // Initialize the first key
    unsigned int val = 42;
    srand(val);

    // Store that as the minimum
    int min = INT_MAX;

    // Use a pseudo-random generator for the other keys
    for (i = 0; i < count; i++) {
        *(key + i) = rand() % 1000;
        // Check for a new min
        if (*(key + i) < min) {
            min = *(key + i);
        }

        h.table[i].val = key + i;
    }

    h.active_entries = i;

    heap_build(&h);

    for (i = 0; i < count; i++) {
        *(key2 + i) = rand() % 1000;

        if (cmp(key2 + i, h.table[0].val) > 0) {
            h.table[0].val = key2 + i;
            heapify(&h, 0);
        }
    }

    // Get the minimum
    int *min_key;
    int *prev_key = &min;

    // Show the real minimum
    // printf("Real min: %d\n", min);

    // Try to get the minimum
    while (heap_delmin(&h, (void **) &min_key)) {
        // Verify that the values are getting larger
        if (*prev_key > *min_key) {
            printf("Previous key is greater than current key!\n");
        }

        prev_key = min_key;
    }

    // Clean up the heap
    heap_destroy(&h);
}