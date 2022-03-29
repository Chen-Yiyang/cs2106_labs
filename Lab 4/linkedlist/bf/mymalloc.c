#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

/** Design Ideas:
- Keep two linked lists - 1) for free partitions, and 2) for used partitions
- Both llists are sorted by start index as key, in ascending order
- TData contains 1 info: val = length of partition
  TNode's key = start idx of partition
- mymalloc: 
    - find first large enough partition in free llist
    - create a new node in used llist, and update the one in free llist by modifying its start pos
- myfree:
    - take out the one to be freed from used llist and put it into free llist, sorted
    - try to merge it with its prev, and its next
*/

TNode *_list = NULL;

TNode* make_partition(int, int, int);
void print_partition(TNode*);

// Self-written utility functions
TNode* make_partition(int idx, int size, int is_free) {
    TData *data = (TData *) malloc(sizeof(TData));
    data->val = size;
    data->is_free = is_free;

    TNode *node = make_node(idx, data);

    return node;
}

void print_partition(TNode* node) {
    printf("Status: %s Start index: %d Length: %d\n", 
            (node->pdata->is_free) ? "FREE" : "ALLOCATED",
            node->key, 
            node->pdata->val);
}

void print_memlist() {
    process_list(_list, print_partition);
}


// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    // initialise
    if (_list == NULL) {
        insert_node(&_list, make_partition(0, MEMSIZE, 1), ASCENDING);
    }

    //printf("Hey %p", _list);

    TNode* best_node = NULL;
    int best_val = -1;

    TNode* curr_node = _list;
    int goal_size = size / sizeof(_heap[0]);

    while (curr_node != NULL) {
        if (!curr_node->pdata->is_free) {
            curr_node = curr_node->next;
            continue;
        } else if (curr_node->pdata->val < goal_size) {
            curr_node = curr_node->next;
            continue;
        }

        if (best_val == -1 || best_val > curr_node->pdata->val) {
            best_val = curr_node->pdata->val;
            best_node = curr_node;
        }

        curr_node = curr_node->next;
        if (curr_node == _list) // reach end, finish one loop alr
            break;
    }

    // cannot fit
    if (best_val == -1) {
        return NULL;
    }

    TNode* new_part;
    if (best_val == goal_size) {
        best_node->pdata->is_free = 0;   
        new_part = best_node;    
    } 
    else {
        new_part = make_partition(best_node->key, goal_size, 0);

        best_node->key += goal_size;
        best_node->pdata->val -= goal_size;

        insert_node(&_list, new_part, ASCENDING);
    }

    return (char *) (&_heap[0] + new_part->key);
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    if (ptr == NULL)
        return; // fail silently

    TNode* curr_node = find_node(_list, get_index(ptr));

    if (curr_node == NULL)
        return; // fail silently

    curr_node->pdata->is_free = 1;

    int curr_end = curr_node->key + curr_node->pdata->val;

    TNode* succ_node = curr_node->next;
    if (succ_node != NULL && succ_node != _list) {
        if (succ_node->key == curr_end && succ_node->pdata->is_free) {
            curr_node->pdata->val += succ_node->pdata->val;

            merge_node(_list, curr_node, SUCCEEDING);
        }
    }

    TNode* prev_node = curr_node->prev;
    if (prev_node != NULL && prev_node != curr_node) {
        int prev_end = prev_node->key + prev_node->pdata->val;
        if (prev_end == curr_node->key && prev_node->pdata->is_free) {
            prev_node->pdata->val += curr_node->pdata->val;
            merge_node(_list, curr_node, PRECEDING);
        }
    }
    
}

