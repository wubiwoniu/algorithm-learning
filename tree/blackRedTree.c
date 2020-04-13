#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node *RBT;
typedef struct node *TNode;
typedef int ItemType;
struct node
{
    int color; //1为红,0为黑
    ItemType value;
    TNode parent;
    TNode left, right;
};