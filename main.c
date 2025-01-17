
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

// This is a comparison function that treats keys as signed ints
int compare_int_keys(register void *key1, register void *key2)
{
    // Cast them as int* and read them in
    register int key1_v = *((int *) key1);
    register int key2_v = *((int *) key2);

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

    // Create the heap
    heap h;
    int count = 10000; // 10M

    heap_create(&h, count, compare_int_keys);

    // Allocate a key and value
    int *key = (int *) malloc(count * sizeof(int));

    // Initialize the first key
    unsigned int val = 42;
    srand(val);

    // Store that as the minimum
    int min = INT_MAX;

    // Use a pseudo-random generator for the other keys
    for (int i = 0; i < count; i++) {
        *(key + i) = rand();
        //printf("i = %d,  val = %d\n", i, *(key + i));

        // Check for a new min
        if (*(key + i) < min) min = *(key + i);

        // Insert into the heap
        heap_insert(&h, key + i);
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