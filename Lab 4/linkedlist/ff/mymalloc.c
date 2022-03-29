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

bool is_not_init;
TNode *_used = NULL;
TNode *_free = NULL;

TNode* make_partition(int, int);
void print_partition(TNode*);

// Self-written utility functions
TNode* make_partition(int idx, int size) {
    TData *data = (TData *) malloc(sizeof(TData));
    data->val = size;

    TNode *node = make_node(idx, data);

    return node;
}

void print_memlist() {
    printf("Free:");
    process_list(_free, print_partition);

    printf("Used:");
    process_list(_used, print_partition);
}

void print_partition(TNode* node) {
    printf("Start index: %d Length: %d\n", node->key, node->pdata->val);
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
    // Do this once
    if (is_not_init) {
        is_not_init = false;

        insert_node(&_free, make_partition(0, MEMSIZE), ASCENDING);
        _used = NULL;
    }

    TNode* curr_node = _free;
    int goal_size = size / sizeof(_heap[0]);
    while (curr_node != NULL) {
        int part_idx = curr_node->key;
        int part_size = curr_node->pdata->val;

        if (part_size >= goal_size) {
            // TODO: create partition
            TNode* new_part = make_partition(part_idx, goal_size);
            insert_node(&_used, new_part, ASCENDING);
            
            if (part_size == goal_size) {
                delete_node(&_free, curr_node);
            } else {
                curr_node->key = part_idx + goal_size;
            }

            return (char *) (&_heap[0] + part_idx);

        } else {
            curr_node = curr_node->next;
        }
    }

    // TODO: Circular?
    // cannot fit
    return NULL;
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    TNode* curr_node_used = find_node(_used, get_index(ptr));
    TNode* curr_node_free = make_partition(curr_node_used->key, curr_node_used->pdata->val);  // create a dup cuz the one in used will be deleted

    delete_node(&_used, curr_node_used);
    insert_node(&_free, curr_node_free, ASCENDING);

    // Merge w. prev. and succ.
    TNode* curr_node = curr_node_free; // create a dup in case the 1st one in free get deleted (merged)
    TNode* prev_node = curr_node->prev;
    if (prev_node->pdata->val == curr_node->key) {
        curr_node = prev_node;
        prev_node->pdata->val = curr_node_free->pdata->val;
        merge_node(_free, curr_node_free, PRECEDING);
    }
    TNode* succ_node = curr_node->next;
    if (curr_node->pdata->val == succ_node->key) {
        curr_node->pdata->val = succ_node->pdata->val;
        merge_node(_free, curr_node, SUCCEEDING);
    }

}

