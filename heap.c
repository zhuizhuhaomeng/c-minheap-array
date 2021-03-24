/**
 * This file defines the methods declared in heap.h
 * These are used to create and manipulate a heap
 * data structure.
 */
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "heap.h"

// Helpful Macros
#define LEFT_CHILD(i) ((i << 1) + 1)
#define RIGHT_CHILD(i) ((i << 1) + 2)
#define PARENT_ENTRY(i) ((i - 1) >> 1)
#define SWAP_ENTRIES(parent, child) \
    {                               \
        void *temp = parent->val;   \
        parent->val = child->val;   \
        child->val = temp;          \
    }

#define GET_ENTRY(index, table) ((heap_entry *)(table + index))


void
heap_create(heap *h, int max_entries, int (*comp_func)(void *, void *))
{
    h->max_entries = max_entries;
    h->cmp = comp_func;
    h->active_entries = 0;
    h->table = malloc(sizeof(void *) * (max_entries + 1));
}


void
heap_destroy(heap *h)
{
    assert(h != NULL);

    free(h->table);

    h->active_entries = 0;
    h->table = NULL;
}


int
heap_size(heap *h)
{
    return h->active_entries;
}


int
heap_min(heap *h, void **val)
{
    if (h->active_entries == 0) {
        return 0;
    }

    heap_entry *root = GET_ENTRY(0, h->table);
    *val = root->val;

    return 1;
}


void
heap_insert(heap *h, void *val)
{
    int parent_index;
    int current_index;
    heap_entry *parent;
    heap_entry *current;
    heap_entry *table;
    heap_cmp_func  cmp;

    assert(h->table != NULL);

    cmp = h->cmp;
    table = h->table;
    current_index = h->active_entries;
    current = GET_ENTRY(current_index, table);

    while (current_index > 0) {
        parent_index = PARENT_ENTRY(current_index);
        parent = GET_ENTRY(parent_index, table);

        if (cmp(val, parent->val) < 0) {
            current->val = parent->val;
            current_index = parent_index;
            current = parent;

        } else {
            break;
        }
    }

    current->val = val;

    h->active_entries++;
}


int
heap_delmin(heap *h, void **val)
{
    int             entries;
    int             current_index;
    int             l, r;
    heap_cmp_func   cmp;
    heap_entry     *last;
    heap_entry     *current;
    heap_entry     *left_child;
    heap_entry     *right_child;
    heap_entry     *table;

    if (h->active_entries == 0) {
        return 0;
    }

    table = h->table;
    current_index = 0;
    current = GET_ENTRY(current_index, table);
    *val = current->val;
    h->active_entries--;
    entries = h->active_entries;

    if (h->active_entries > 0) {
        last = GET_ENTRY(entries, table);
        current->val = last->val;

        cmp = h->cmp;
        while (1) {
            l = LEFT_CHILD(current_index);
            r = RIGHT_CHILD(current_index);
            if (l >= entries) {
                break;
            }

            left_child = GET_ENTRY(l, table);

            /* We have a left + right child */
            if (r < entries) {
                right_child = GET_ENTRY((r), table);

                if (cmp(left_child->val, right_child->val) <= 0) {

                    // Swap with the left if it is smaller
                    if (cmp(current->val, left_child->val) > 0) {
                        SWAP_ENTRIES(current, left_child);
                        current_index = l;
                        current = left_child;

                    } else {
                        // Otherwise, the current is smaller
                        break;
                    }

                    // Right child is smaller
                } else {
                    // Swap with the right if it is smaller
                    if (cmp(current->val, right_child->val) > 0) {
                        SWAP_ENTRIES(current, right_child);
                        current_index = r;
                        current = right_child;

                    } else {
                        // Current is smaller
                        break;
                    }
                }

                // We only have a left child, only do something if the left is smaller
            } else if (cmp(current->val, left_child->val) == 1) {
                SWAP_ENTRIES(current, left_child);
                current_index = l;
                current = left_child;

                // Done otherwise
            } else {
                break;
            }
        }
    }

    return 1;
}


/* Allows a user to iterate over all entries, e.g. to free() the memory */
void
heap_foreach(heap* h, void (*func)(void *))
{
    // Store the current index and max index
    int index = 0;
    int entries = h->active_entries;

    heap_entry* entry;
    heap_entry* table = h->table;

    for (; index < entries; index++) {
        // Get the entry
        entry = GET_ENTRY(index, table);

        // Call the user function
        func(entry->val);
    }
}


void
heapify(heap *h, int i)
{
    int             entries;
    int             smallest;
    int             l, r;
    void           *tmp_val;
    heap_cmp_func   cmp;
    heap_entry     *table;

    if (h->active_entries == 0) {
        return;
    }

    cmp = h->cmp;
    table = h->table;
    entries = h->active_entries;
    smallest = i;

    while (1) {
        l = LEFT_CHILD(smallest);
        r = RIGHT_CHILD(smallest);

        if (l < entries && cmp(table[l].val, table[smallest].val) < 0) {
            smallest = l;
        }

        if (r < entries && cmp(table[r].val, table[smallest].val) < 0) {
            smallest = r;
        }

        if (smallest == i) {
            break;
        }

        tmp_val = table[i].val;
        table[i].val = table[smallest].val;
        table[smallest].val = tmp_val;
        i = smallest;
    }

    return;
}


void
heap_build(heap *h)
{
    int i;

    for (i = h->active_entries / 2 - 1; i >= 0; i--) {
        heapify(h, i);
    }
}