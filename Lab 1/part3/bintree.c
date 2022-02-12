#include <stdlib.h>
#include <stdio.h>
#include "bintree.h"

/* ------------------------- DO NOT IMPLEMENT THESE ----------------------

These are provided "free" to you, and you do not need to implement them,
though you MAY  need to understand what they do. Some give you hints
on how to work with pointers to pointers. You MAY find some of these
functions useful.

  ------------------------------------------------------------------------- */

// Searches for the node containing "name" in the tree whose root is at "root", 
// returning both the node ifself in "node", and its parent in "prevnode" 
// (useful for deleting node). Both "node" and "prevnode" are set to NULL
// if name is not found in the tree.

void findNode(char *name, TTreeNode *root, TTreeNode **node, TTreeNode **prevnode) {

    TTreeNode *trav = root;
    TTreeNode *prev = NULL;

    while(trav != NULL) {
        int cmp = strcmp(trav->name, name);

        if(cmp == 0)
        {
            *node = trav;
            *prevnode = prev;
            return;
        }
        
        prev = trav;
        if(cmp < 0)
            trav = trav->right;
        else
            trav = trav->left;
    }

    *node = NULL;
    *prevnode = NULL;
}

// Searches for the node with the smallest value ("smallest node") in the tree originating at "node".
// Returns the smallest node and its parent in "smallest_node" and "parent"
// respectively.
void findSmallest(TTreeNode *node, TTreeNode **smallest_node, TTreeNode **parent) {
    TTreeNode *trav = node;
    TTreeNode *prev = NULL;

    if(trav == NULL)
        return;

    while(trav->left != NULL)
    {
        prev = trav;
        trav = trav->left;
    }

    *smallest_node = trav;
    *parent = prev;
}

// Delete a node at "node" with parent node "prevnode"
void delNode(TTreeNode *node, TTreeNode *prevnode) {
    // This is a leaf node
    if(node->left == NULL && node->right == NULL) {
        // See whether node is on parent's left or right, and NULL the
        // corresponding pointer

        int cmp = strcmp(prevnode->name, node->name);

        // Previous node is smaller than this one. This
        // node is on the right
        if(cmp < 0) 
            prevnode->right = NULL;
        else
            prevnode->left = NULL;

        freenode(node);

        return;
    }

    // This has a child on the left only
    if(node->right == NULL) {
        // Copy the right child over
        node = node->left;
        freenode(node->left);
        return;
    }

    // This has a child on the right only
    if(node->left == NULL) {
        node = node->right;
        freenode(node->right);
        return;
    }

    // This has children on both nodes
    TTreeNode *smallest, *smallest_parent;
    findSmallest(node->right, &smallest, &smallest_parent);
    node = smallest;
    smallest_parent->left = NULL;
    freenode(smallest);
}


/* ---------------------------- IMPLEMENT THESE --------------------------- */

// Create a new node with name set to "name" and
// phoneNum set to "phoneNum".
void delTree(TTreeNode *root) {
    // Implement deleting the entire tree, whose
    // root is at "root".
    if (root == NULL) return;
    // delete the entire tree using postOrder
    if (root->left != NULL) {
    	delTree(root->left);
    }
    if (root->right != NULL) {
    	delTree(root->right);
    }

    // delete this node
    freenode(root);
}

TTreeNode *makeNewNode(char *name, char *phoneNum) {
    // Implement makeNewNode to create a new
    // TTreeNode containing name and phoneNum
    TTreeNode *node = (TTreeNode *) malloc(sizeof(TTreeNode));
    node->name = (char *) malloc(strlen(name) + 1); // for '\0'
    //node->phoneNum = (char *) malloc(9 * sizeof(char));

    strcpy(node->name, name);
    strcpy(node->phoneNum, phoneNum);
    node->left = NULL;
    node->right = NULL;

    return node;
}

// Add a new node to the tree. 
// Note that "root" is a POINTER to the tree's root,
// not the root itself.

void addNode(TTreeNode **root, TTreeNode *node) {

    // Add a new node to the tree, where root is
    // the POINTER to the tree's root.
    
    // first update, assign to root
    if (*root == NULL) {
        *root = node;
    	return;
    }

    TTreeNode *curr = *root;
    
    while (curr != NULL) {
    	int cmpRes = strcmp(node->name, curr->name);

	if (cmpRes <= 0) {
	    if (curr->left != NULL) {
	    	curr = curr->left;
	    } else {
	        curr->left = node;
		return;
	    }
	} else {
	    if (curr->right != NULL) {
	    	curr = curr->right;
	    } else {
	    	curr->right = node;
		return;
	    }
	}
    }

}

void freenode(TTreeNode *node) {
    // Frees the memory used by node.
    if (node == NULL) return;
	
    if (node->name != NULL) {
        free(node->name);
	node->name = NULL;
    }

    free(node);
    node = NULL;
}

void print_inorder(TTreeNode *node) {
    // Implement in-order printing of the tree
    // Recursion is probably best here.
    if (node == NULL) {
    	printf("The phone book is empty.\n");
	return;
    }

    if (node->left != NULL) print_inorder(node->left);
    printf("Name: %s, phone number: %s\n", node->name, node->phoneNum);
    if (node->right != NULL) print_inorder(node->right);
    
}
