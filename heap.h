/*
 * Author: Armon Dadgar
 *
 * Header for the Heap functions and data definitions
 */

#ifndef HEAP_H
#define HEAP_H

// Structure for a single heap entry
typedef struct heap_entry
{
    void *val; // Key for this entry
} heap_entry;

typedef int (*heap_cmp_func)(void *, void *);
// Main struct for representing the heap
typedef struct heap
{
    heap_cmp_func cmp; // The key comparison function to use
    int max_entries;
    int active_entries;  // The number of entries in the heap
    heap_entry *table;   // Pointer to the table, which maps to the pages
} heap;


void heap_create(heap *h, int initial_size, heap_cmp_func cmp);
int heap_size(heap *h);
void heap_insert(heap *h, void *val);
int heap_min(heap *h, void **val);
int heap_delmin(heap *h, void **val);
void heap_foreach(heap *h, void (*func)(void *));
void heap_destroy(heap *h);
void heap_build(heap *h);
void heapify(heap *h, int i);

#endif
