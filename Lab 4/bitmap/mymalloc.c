#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"

char _heap[MEMSIZE] = {0};
unsigned char map[MEMSIZE / 8] = {0};
int num_zeroes_arr[MEMSIZE] = {0};

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
    // Implement this to call print_map from bitmap.c
    print_map(map, MEMSIZE / 8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    int len = MEMSIZE / 8;
    long num_zeroes = size / sizeof(_heap[0]);

    long idx = search_map(map, len, num_zeroes);

    if (idx == -1)
        return NULL;

    allocate_map(&map[0], idx, num_zeroes);
    num_zeroes_arr[idx] = num_zeroes;

    char *p = (char *) (&_heap[0] + idx);
    return p;
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    if (ptr == NULL)
        return;
    
    long idx = get_index(ptr);
    free_map(&map[0], idx, num_zeroes_arr[idx]);
    num_zeroes_arr[idx] = 0;
}

