#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct node *TNode;
typedef struct tree *RB_tree;
typedef int ItemType;
struct node
{
    int color; 
    ItemType value;
    TNode parent;
    TNode left, right;
};

struct tree
{
    TNode root;
    TNode nil;
};

void left_rotate(RB_tree tree, TNode node);
void right_rotate(RB_tree tree, TNode node);
void preorder_tree_walker(RB_tree tree, TNode node);
void inorder_tree_walker(RB_tree tree, TNode node);
void postorder_tree_walker(RB_tree tree, TNode node);
TNode tree_minimum(RB_tree tree, TNode node);
TNode tree_maximum(RB_tree tree, TNode node);
TNode tree_successor(RB_tree tree, TNode node);
TNode tree_predecessor(RB_tree tree, TNode node);
TNode tree_search(RB_tree tree, ItemType key);
RB_tree tree_create();
RB_tree tree_insert(RB_tree tree, ItemType value);
RB_tree tree_insert_fixup(RB_tree tree, TNode node);
RB_tree tree_delete_fixup(RB_tree tree, TNode node);
RB_tree transplant(RB_tree tree, TNode u, TNode v);
RB_tree tree_delete(RB_tree tree, ItemType key);

int main()
{
    RB_tree tree = tree_create();
    int array[] = {0, 6, 192, -6, 41, 4, 1, 8, 5, 98, -19, 85, 12, 2, 7};
    for (int i = 0; i < 15; i++)
    {
        tree = tree_insert(tree, array[i]);
    }
    preorder_tree_walker(tree, tree->root);
    inorder_tree_walker(tree, tree->root);

    for (int i = 0; i < 15; i++)
    {
        tree = tree_delete(tree, array[i]);
        preorder_tree_walker(tree, tree->root);
        inorder_tree_walker(tree, tree->root);
    }
    system("PAUSE");
}

RB_tree tree_create()
{
    RB_tree tree = (RB_tree)malloc(sizeof(struct tree));
    tree->nil = (TNode)malloc(sizeof(struct node));
    tree->nil->color = 0;
    tree->root = tree->nil;
    return tree;
}

TNode tree_minimum(RB_tree tree, TNode node)
{
    while (node->left != tree->nil)
        node = node->left;
    return node;
}

TNode tree_maximum(RB_tree tree, TNode node)
{
    while (node->right != tree->nil)
        node = node->right;
    return node;
}

TNode tree_successor(RB_tree tree, TNode node)
{
    if (node->right != tree->nil)
        return tree_minimum(tree, node->right);
    TNode temp = node->parent;
    while (temp != tree->nil && node == temp->right)
    {
        node = temp;
        temp = node->parent;
    }
    return temp;
}

TNode tree_predecessor(RB_tree tree, TNode node)
{
    if (node->left != tree->nil)
        return tree_maximum(tree, node->left);
    TNode temp = node->parent;
    while (temp != tree->nil && node == temp->left)
    {
        node = temp;
        temp = node->parent;
    }
    return temp;
}

TNode tree_search(RB_tree tree, ItemType key)
{
    TNode node = tree->root;
    while (node != tree->nil && node->value != key)
    {
        if (node->value > key)
            node = node->left;
        else if (node->value < key)
            node = node->right;
    }
    printf("search = %d\n", node->value);
    return node;
}

void left_rotate(RB_tree tree, TNode node)
{
    TNode rc = node->right;
    if (rc != tree->nil)
    {
        node->right = rc->left;
        if (rc->left != tree->nil)
            rc->left->parent = node;
        rc->parent = node->parent;
        if (node == tree->root)
            tree->root = rc;
        else if (node == node->parent->left)
            node->parent->left = rc;
        else
            node->parent->right = rc;
        node->parent = rc;
        rc->left = node;
    }
}

void right_rotate(RB_tree tree, TNode node)
{
    TNode lc = node->left;
    if (lc != tree->nil)
    {
        node->left = lc->right;
        if (lc->right != tree->nil)
            lc->right->parent = node;
        lc->parent = node->parent;
        if (node == tree->root)
            tree->root = lc;
        else if (node == node->parent->left)
            node->parent->left = lc;
        else
            node->parent->right = lc;
        node->parent = lc;
        lc->right = node;
    }
}

RB_tree tree_insert(RB_tree tree, ItemType value)
{
    TNode node = tree->root;
    TNode temp = tree->nil;
    while (node != tree->nil)
    {
        temp = node;
        if (node->value > value)
            node = node->left;
        else
            node = node->right;
    }
    TNode new_node = (TNode)malloc(sizeof(struct node));
    new_node->value = value;
    new_node->color = 1;
    new_node->parent = temp;
    new_node->left = new_node->right = tree->nil;
    if (temp == tree->nil)
        tree->root = new_node;
    else if (temp->value > value)
        temp->left = new_node;
    else
        temp->right = new_node;
    tree = tree_insert_fixup(tree, new_node);
    return tree;
}

RB_tree tree_insert_fixup(RB_tree tree, TNode node)
{
    while (node->parent->color == 1)
    {
        if (node->parent == node->parent->parent->left)
        {
            TNode prc = node->parent->parent->right;
            if (prc->color == 1)
            {
                node->parent->color = 0;
                prc->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    left_rotate(tree, node);
                }
                node->parent->color = 0;
                node->parent->parent->color = 1;
                right_rotate(tree, node->parent->parent);
            }
        }
        else
        {
            TNode plc = node->parent->parent->left;
            if (plc->color == 1)
            {
                node->parent->color = 0;
                plc->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    right_rotate(tree, node);
                }
                node->parent->color = 0;
                node->parent->parent->color = 1;
                left_rotate(tree, node->parent->parent);
            }
        }
    }
    tree->root->color = 0;
    return tree;
}

RB_tree transplant(RB_tree tree, TNode u, TNode v)
{
    if (u->parent == tree->nil)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
    return tree;
}

RB_tree tree_delete_fixup(RB_tree tree, TNode node)
{
    while (node != tree->root && node->color == 0)
    {
        if (node == node->parent->left)
        {
            TNode rb = node->parent->right;
            printf("rb = %d\n", rb->color);
            if (rb->color == 1)
            {
                rb->color = 0;
                node->parent->color = 1;
                left_rotate(tree, node->parent);
                rb = node->parent->right;
            }
            if (rb->left->color == 0 && rb->right->color == 0)
            {
                rb->color = 1;
                node = node->parent;
            }
            else
            {
                if (rb->right->color == 0)
                {
                    rb->left->color = 0;
                    rb->color = 1;
                    right_rotate(tree, rb);
                    rb = node->parent->right;
                }
                rb->color = node->parent->color;
                node->parent->color = 0;
                rb->right->color = 0;
                left_rotate(tree, node->parent);
                node = tree->root;
            }
        }
        else
        {
            TNode lb = node->parent->left;
            printf("lb = %d\n", lb->color);
            if (lb->color == 1)
            {
                lb->color = 0;
                node->parent->color = 1;
                right_rotate(tree, node->parent);
                lb = node->parent->left;
            }
            if (lb->left->color == 0 && lb->right->color == 0)
            {
                lb->color = 1;
                node = node->parent;
            }
            else
            {
                if (lb->left->color == 0)
                {
                    lb->right->color = 0;
                    lb->color = 1;
                    left_rotate(tree, lb);
                    lb = node->parent->left;
                }
                lb->color = node->parent->color;
                node->parent->color = 0;
                lb->left->color = 0;
                right_rotate(tree, node->parent);
                node = tree->root;
            }
        }
    }
    node->color = 0;
    return tree;
}

RB_tree tree_delete(RB_tree tree, ItemType key)
{
    TNode temp, check;
    TNode del_one = tree_search(tree, key);
    temp = del_one;
    int origin_color = del_one->color;
    if (del_one->left == tree->nil)
    {
        transplant(tree, del_one, del_one->right);
        check = del_one->right;
    }
    else if (del_one->right == tree->nil)
    {
        transplant(tree, del_one, del_one->left);
        check = del_one->left;
    }
    else
    {
        temp = tree_predecessor(tree, del_one);
        origin_color = temp->color;
        check = temp->left;
        if (temp == del_one->left)
        {
            check->parent = temp;
        }
        else
        {
            transplant(tree, temp, temp->left);
            temp->left = del_one->left;
            temp->left->parent = temp;
        }
        transplant(tree, del_one, temp);
        temp->right = del_one->right;
        temp->right->parent = temp;
        temp->color = del_one->color;
    }
    free(del_one);
    if (origin_color == 0)
        tree = tree_delete_fixup(tree, check);
    return tree;
}

void preorder_tree_walker(RB_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        printf("%d  %d\n", node->value, node->color);
        preorder_tree_walker(tree, node->left);
        preorder_tree_walker(tree, node->right);
    }
}
void inorder_tree_walker(RB_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        inorder_tree_walker(tree, node->left);
        printf("%d\n", node->value);
        inorder_tree_walker(tree, node->right);
    }
}
void postorder_tree_walker(RB_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        postorder_tree_walker(tree, node->left);
        postorder_tree_walker(tree, node->right);
        printf("%d  %d\n", node->value, node->color);
    }
}