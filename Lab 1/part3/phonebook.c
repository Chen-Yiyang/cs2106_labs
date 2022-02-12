#include <stdio.h>
#include "phonebook.h"
#include "bintree.h"


static TTreeNode *_root = NULL;

char *findPerson(char *name) {
    TTreeNode *node, *prev;
    findNode(name, _root, &node, &prev);

    if(node != NULL)
        return node->phoneNum;
    else
        return NULL;
}

void print_phonebook() {
    print_inorder(_root);
}

void addPerson(char *name, char *phoneNum) {
    if(findPerson(name) == NULL) 
    {
        TTreeNode *node = makeNewNode(name, phoneNum);
        addNode(&_root, node);
    }
    else
        printf("%s is already in phonebook.\n", name);
}

void delPerson(char *name) {
    TTreeNode *node, *prevnode;
    findNode(name, _root, &node, &prevnode);
    if(node == NULL)
    {
        printf("Unable to find %s\n", name);
        return;
    }

    delNode(node, prevnode);
}

void delPhonebook() {
    delTree(_root);
    _root = NULL;
}

