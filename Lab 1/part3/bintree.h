#include <string.h>
#include <stdlib.h>

typedef struct tn {
    char *name;
    char phoneNum[9];
    struct tn *left, *right;
} TTreeNode;

void findNode(char *, TTreeNode *, TTreeNode **, TTreeNode **);
void findSmallest(TTreeNode *, TTreeNode **, TTreeNode **);
void delNode(TTreeNode *, TTreeNode *);
void delTree(TTreeNode *);
TTreeNode *makeNewNode(char *, char *);
void addNode(TTreeNode **, TTreeNode *);
void freenode(TTreeNode *);
void print_inorder(TTreeNode *);
