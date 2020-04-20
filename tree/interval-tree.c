#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define min -10000005

typedef struct node *TNode;
typedef struct tree *interval_tree;
typedef int ItemType;
struct node
{
    int color;
    //该节点对应的区间,当区间a与区间b重叠时有a.low<=b.high&&a.high>=b.low
    ItemType low, high; //low是key
    //以该节点为根节点的子树的区间最大值,即区间的最右侧,同时可知
    //node->max = max(node->max, max(node->left->max, node->right->max))
    ItemType max;
    TNode parent;
    TNode left, right;
};

struct tree
{
    TNode root;
    TNode nil;
};

void left_rotate(interval_tree tree, TNode node);
void right_rotate(interval_tree tree, TNode node);
void preorder_tree_walker(interval_tree tree, TNode node);
void inorder_tree_walker(interval_tree tree, TNode node);
void postorder_tree_walker(interval_tree tree, TNode node);
bool interval_overlap(TNode node, ItemType low, ItemType high);
int set_max(int a, int b, int c);
TNode tree_minimum(interval_tree tree, TNode node);
TNode tree_maximum(interval_tree tree, TNode node);
TNode tree_successor(interval_tree tree, TNode node);
TNode tree_predecessor(interval_tree tree, TNode node);
TNode tree_search(interval_tree tree, ItemType low, ItemType high); //查询与low~high重叠的那个节点
interval_tree tree_create();
interval_tree tree_insert(interval_tree tree, ItemType low, ItemType high);
interval_tree tree_insert_fixup(interval_tree tree, TNode node);
interval_tree tree_delete_fixup(interval_tree tree, TNode node);
interval_tree tree_max_fixup(interval_tree tree, TNode node);
interval_tree transplant(interval_tree tree, TNode u, TNode v);
interval_tree tree_delete(interval_tree tree, TNode node);

int main()
{
    interval_tree tree = tree_create();
    int array[] = {6, 10, 0, 3, 5, 8, 26, 26, 15, 23, 25, 30, 8, 9, 16, 21, 17, 19, 19, 20};
    for (int i = 0; i < 20; i += 2)
    {
        tree = tree_insert(tree, array[i], array[i + 1]);
    }
    preorder_tree_walker(tree, tree->root);
    inorder_tree_walker(tree, tree->root);

    for (int i = 0; i < 20; i += 2)
    {
        TNode node = tree_search(tree, array[i], array[i + 1]);
        printf("delete low = %d, high = %d\n", node->low, node->high);
        tree = tree_delete(tree, node);
        preorder_tree_walker(tree, tree->root);
        inorder_tree_walker(tree, tree->root);
    }
    system("PAUSE");
}

bool interval_overlap(TNode node, ItemType low, ItemType high)
{
    if (node->low <= high && node->high >= low)
        return true;
    return false;
}

int set_max(int a, int b, int c)
{
    a = a > b ? a : b;
    return a > c ? a : c;
}

interval_tree tree_create()
{
    interval_tree tree = (interval_tree)malloc(sizeof(struct tree));
    tree->nil = (TNode)malloc(sizeof(struct node));
    tree->nil->color = 0;
    tree->nil->max = min;
    tree->root = tree->nil;
    return tree;
}

TNode tree_minimum(interval_tree tree, TNode node)
{
    while (node->left != tree->nil)
        node = node->left;
    return node;
}

TNode tree_maximum(interval_tree tree, TNode node)
{
    while (node->right != tree->nil)
        node = node->right;
    return node;
}

TNode tree_successor(interval_tree tree, TNode node)
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

TNode tree_predecessor(interval_tree tree, TNode node)
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

TNode tree_search(interval_tree tree, ItemType low, ItemType high)
{
    TNode node = tree->root;
    //不为空且不重叠则继续向下找
    while (node != tree->nil && !interval_overlap(node, low, high))
    {
        //条件1:左子树不为空
        //条件2:左子树代表的区间最右端大于查找区间的最左端
        //如果左子树max大于low的话有可能在左子树找到重叠的区间
        if (node->left != tree->nil && node->left->max >= low)
            node = node->left;
        else
            node = node->right;
    }
    //printf("search = %d\n", node->value);
    return node;
}

void left_rotate(interval_tree tree, TNode node)
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
    //注释在右旋里
    rc->max = node->max;
    node->max = set_max(node->high, node->left->max, node->right->max);
}

void right_rotate(interval_tree tree, TNode node)
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
    //和插入时与max比不一样,因为旋转时结构已经改变了,max并不具备意义,同时lc代替了node
    //所以node的原max就是lc的现max
    lc->max = node->max;
    node->max = set_max(node->high, node->left->max, node->right->max);
}

interval_tree tree_insert(interval_tree tree, ItemType low, ItemType high)
{
    TNode node = tree->root;
    TNode temp = tree->nil;
    //插入时每一层都要更新一遍max
    while (node != tree->nil)
    {
        temp = node;
        node->max = set_max(node->max, high, min);
        if (node->low > low)
            node = node->left;
        else
            node = node->right;
    }
    TNode new_node = (TNode)malloc(sizeof(struct node));
    new_node->low = low;
    new_node->high = high;
    new_node->max = high;
    new_node->color = 1;
    new_node->parent = temp;
    new_node->left = new_node->right = tree->nil;
    if (temp == tree->nil)
        tree->root = new_node;
    else if (temp->low > low)
        temp->left = new_node;
    else
        temp->right = new_node;
    tree = tree_insert_fixup(tree, new_node);
    return tree;
}

interval_tree tree_insert_fixup(interval_tree tree, TNode node)
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

interval_tree transplant(interval_tree tree, TNode u, TNode v)
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

interval_tree tree_delete_fixup(interval_tree tree, TNode node)
{
    while (node != tree->root && node->color == 0)
    {
        if (node == node->parent->left)
        {
            TNode rb = node->parent->right;
            //printf("rb = %d\n", rb->color);
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
            //printf("lb = %d\n", lb->color);
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

interval_tree tree_max_fixup(interval_tree tree, TNode node)
{
    while (node != tree->nil)
    {
        node->max = set_max(node->high, node->left->max, node->right->max);
        node = node->parent;
    }
    return tree;
}

interval_tree tree_delete(interval_tree tree, TNode node)
{
    TNode temp, check; //从check向上所有节点的max都需要更新
    TNode del_one = node;
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
    tree = tree_max_fixup(tree, check->parent);
    if (origin_color == 0)
        tree = tree_delete_fixup(tree, check);
    return tree;
}

void preorder_tree_walker(interval_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        printf("%d  %d max = %d ", node->low, node->high, node->max);
        if (node->color == 0)
            printf("color = black\n");
        else
            printf("color = red\n");
        preorder_tree_walker(tree, node->left);
        preorder_tree_walker(tree, node->right);
    }
}
void inorder_tree_walker(interval_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        inorder_tree_walker(tree, node->left);
        printf("%d\n", node->low);
        inorder_tree_walker(tree, node->right);
    }
}
void postorder_tree_walker(interval_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        postorder_tree_walker(tree, node->left);
        postorder_tree_walker(tree, node->right);
        printf("%d  %d\n", node->low, node->high);
    }
}