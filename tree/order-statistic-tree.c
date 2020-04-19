#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct node *TNode;
typedef struct tree *os_tree;
typedef int ItemType;
struct node
{
    int color;
    ItemType value;
    TNode parent;
    TNode left, right;
    int size; //以当前节点为根节点的子树的大小(包含自身)
};

struct tree
{
    TNode root;
    TNode nil;
};

int os_rank(os_tree tree, TNode node); //返回node在树中序遍历后的索引
void left_rotate(os_tree tree, TNode node);
void right_rotate(os_tree tree, TNode node);
void preorder_tree_walker(os_tree tree, TNode node);
void inorder_tree_walker(os_tree tree, TNode node);
void postorder_tree_walker(os_tree tree, TNode node);
TNode os_select(os_tree tree, int rank); //返回中序遍历后索引为rank的节点
TNode tree_minimum(os_tree tree, TNode node);
TNode tree_maximum(os_tree tree, TNode node);
TNode tree_successor(os_tree tree, TNode node);
TNode tree_predecessor(os_tree tree, TNode node);
TNode tree_search(os_tree tree, ItemType key);
os_tree tree_create();
os_tree tree_insert(os_tree tree, ItemType value);
os_tree tree_insert_fixup(os_tree tree, TNode node);
os_tree tree_delete_fixup(os_tree tree, TNode node);
os_tree os_size_fixup(os_tree tree, TNode node);
os_tree transplant(os_tree tree, TNode u, TNode v);
os_tree tree_delete(os_tree tree, ItemType key);

int main()
{
    os_tree tree = tree_create();
    int array[] = {0, 6, 192, -6, 41, 4, 1, 8, 5, 98, -19, 85, 12, 2, 7};
    for (int i = 0; i < 15; i++)
    {
        tree = tree_insert(tree, array[i]);
    }
    preorder_tree_walker(tree, tree->root);
    inorder_tree_walker(tree, tree->root);

    TNode node = os_select(tree, 6);
    //printf("node rank = %d\n", node->value);
    printf("rank6 = %d\n", os_rank(tree, node));
    node = os_select(tree, 1);
    printf("rank1 = %d\n", os_rank(tree, node));
    for (int i = 0; i < 15; i++)
    {
        tree = tree_delete(tree, array[i]);
        preorder_tree_walker(tree, tree->root);
        inorder_tree_walker(tree, tree->root);
    }
    system("PAUSE");
}

int os_rank(os_tree tree, TNode node)
{
    int rank = node->left->size + 1; //node在以node为根的子树中的索引
    while (node != tree->root)       //一直向上找直至root,因为到root就无法继续向上,此时的索引就是node的索引
    {
        //如果node是父节点的右子树,意味着向上的时候会添加node->parent->left->size+1个节点在其前面
        //因为左子树在中序遍历时始终先于右子树,其父节点也是一样
        if (node == node->parent->right)
            rank += node->parent->left->size + 1;
        node = node->parent;
    }
    return rank;
}

os_tree tree_create()
{
    os_tree tree = (os_tree)malloc(sizeof(struct tree));
    tree->nil = (TNode)malloc(sizeof(struct node));
    tree->nil->size = 0;
    tree->nil->color = 0;
    tree->root = tree->nil;
    return tree;
}

TNode os_select(os_tree tree, int rank)
{
    TNode node = tree->root;
    int r = node->left->size + 1; //当前node的rank=左子树大小+1
    while (r != rank && node != tree->nil)
    {
        if (r < rank) //往右子树找,由于已经有r个节点在前了,所以rank=rank-r
        {
            rank = rank - r;
            node = node->right;
        }
        else
        {
            node = node->left;
        }
        r = node->left->size + 1;
    }
    return node;
}

TNode tree_minimum(os_tree tree, TNode node)
{
    while (node->left != tree->nil)
        node = node->left;
    return node;
}

TNode tree_maximum(os_tree tree, TNode node)
{
    while (node->right != tree->nil)
        node = node->right;
    return node;
}

TNode tree_successor(os_tree tree, TNode node)
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

TNode tree_predecessor(os_tree tree, TNode node)
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

TNode tree_search(os_tree tree, ItemType key)
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

void left_rotate(os_tree tree, TNode node)
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
    rc->size = node->size;
    node->size = (node->left->size) + (node->right->size) + 1;
}

void right_rotate(os_tree tree, TNode node)
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
    lc->size = node->size;
    node->size = (node->left->size) + (node->right->size) + 1;
}

os_tree tree_insert(os_tree tree, ItemType value)
{
    TNode node = tree->root;
    TNode temp = tree->nil;
    while (node != tree->nil)
    {
        temp = node;
        node->size += 1;
        if (node->value > value)
            node = node->left;
        else
            node = node->right;
    }
    TNode new_node = (TNode)malloc(sizeof(struct node));
    new_node->value = value;
    new_node->color = 1;
    new_node->parent = temp;
    new_node->size = 1;
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

os_tree tree_insert_fixup(os_tree tree, TNode node)
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

os_tree transplant(os_tree tree, TNode u, TNode v)
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

os_tree tree_delete_fixup(os_tree tree, TNode node)
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

os_tree os_size_fixup(os_tree tree, TNode node)
{
    while (node != tree->nil)
    {
        //printf("now = %d\n", node->size);
        node->size -= 1;
        node = node->parent;
    }
    return tree;
}

os_tree tree_delete(os_tree tree, ItemType key)
{
    TNode temp, check; //从check(不包括check)到根节点所有节点size都需-1,因为都删除或移出了一个节点
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
        temp->size = del_one->size; //代替了后size也继承
    }
    free(del_one);
    tree = os_size_fixup(tree, check->parent);
    if (origin_color == 0)
        tree = tree_delete_fixup(tree, check);
    return tree;
}

void preorder_tree_walker(os_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        printf("%d  %d\n", node->value, node->size);
        preorder_tree_walker(tree, node->left);
        preorder_tree_walker(tree, node->right);
    }
    else
    {
        printf("nil\n");
    }
}
void inorder_tree_walker(os_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        inorder_tree_walker(tree, node->left);
        printf("%d\n", node->value);
        inorder_tree_walker(tree, node->right);
    }
}
void postorder_tree_walker(os_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        postorder_tree_walker(tree, node->left);
        postorder_tree_walker(tree, node->right);
        printf("%d  %d\n", node->value, node->color);
    }
}