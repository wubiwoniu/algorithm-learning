#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//二叉搜索树节点x左子树节点键值均大于等于x键值，右子树反之
typedef struct node *BST;
typedef int ItemType;
struct node
{
    ItemType value;
    BST parent;
    BST left, right;
};

BST create_node();                             //malloc后返回指针
BST tree_maximum(BST root);                    //以root为根的子树的最大值
BST tree_minimum(BST root);                    //以root为根的子树的最小值
BST tree_successor(BST node);                  //node节点的后继
BST tree_predecessor(BST node);                //node节点的前驱
BST tree_search_recur(BST root, ItemType key); //递归查找以root为根的子树中键值为key的节点
BST tree_search(BST root, ItemType key);       //非递归版
BST tree_insert(BST root, ItemType value);     //插入,需要将形参返回
BST tree_delete(BST root, ItemType key);       //同理↓
BST transplant(BST root, BST u, BST v);        //u的父节点指向v,v指向u的父节点,因为可能修改根节点,所以返回根节点
void preorder_tree_walker(BST node);
void inorder_tree_walker(BST node);
void postorder_tree_walker(BST node);

int main()
{
    BST root = NULL;
    int array[] = {2, 0, -1, 1, 4, 3, 6, 5};
    for (int i = 0; i < 8; i++)
    {
        root = tree_insert(root, array[i]);
        //printf("%d\n", root->value);
    }
    inorder_tree_walker(root);
    printf("\n");
    //preorder_tree_walker(root);
    //printf("\n");
    BST Max = tree_maximum(root);
    BST Min = tree_minimum(root);
    BST next = tree_successor(Min);
    BST prev = tree_predecessor(Max);
    printf("%d\n", Max->value);
    printf("%d\n", Min->value);
    printf("%d\n", next->value);
    printf("%d\n\n", prev->value);

    root = tree_delete(root, 4);
    root = tree_delete(root, Min->value);

    inorder_tree_walker(root);
    printf("\n");
    //preorder_tree_walker(root);
    //printf("\n");
    Max = tree_maximum(root);
    Min = tree_minimum(root);
    next = tree_successor(Min);
    prev = tree_predecessor(Max);
    printf("%d\n", Max->value);
    printf("%d\n", Min->value);
    printf("%d\n", next->value);
    printf("%d\n\n", prev->value);

    while (root != NULL)
    {
        root = tree_delete(root, tree_maximum(root)->value);
        inorder_tree_walker(root);
        printf("\n");
    }
    system("PAUSE");
}

BST create_node()
{
    BST node = (BST)malloc(sizeof(struct node));
    return node;
}

BST tree_maximum(BST root)
{
    BST node = root;
    while (node->right != NULL)
        node = node->right;
    return node;
}

BST tree_minimum(BST root)
{
    BST node = root;
    while (node->left != NULL)
        node = node->left;
    return node;
}

BST tree_successor(BST node)
{
    BST ptr = node;
    /*  如果有右子树，因为右子树键值均大于node，所以只需要找到右子树最小节点，
        至于为什么不向上找，假设当前节点node在某节点左子树，如果该节点或该节点的任意祖先节点
        有右子树，这些右子树的键值都只会比node右子树大，因为有
        key(node)<key(node->right)<key(node->p)<key(node->p->right)
    */
    if (ptr->right != NULL)
        return tree_minimum(ptr->right);
    /*  如果没有右子树，只能向上找，如果当前查询节点是其父节点右节点则有key(node)>key(node->p及其左子树)  
        当是左节点时，则有key(node及其所有子孙节点)<key(node->p)
    */
    BST temp = ptr->parent;
    while (temp != NULL && ptr == temp->right)
    {
        ptr = temp;
        temp = ptr->parent;
    }
    return temp;
}

BST tree_predecessor(BST node)
{
    BST ptr = node;
    if (ptr->left != NULL)
        return tree_maximum(ptr->left);
    BST temp = ptr->parent;
    while (temp != NULL && ptr == temp->left)
    {
        ptr = temp;
        temp = ptr->parent;
    }
    return temp;
}

BST tree_search_recur(BST root, ItemType key)
{
    if (root == NULL || root->value == key)
        return root;
    else if (root->value > key)
        return tree_search_recur(root->left, key);
    else
        return tree_search_recur(root->right, key);
}

BST tree_search(BST root, ItemType key)
{
    BST node = root;
    while (node != NULL && node->value != key)
    {
        if (node->value > key)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

BST tree_insert(BST root, ItemType value)
{
    BST node = root;
    BST temp = NULL;
    while (node != NULL)
    {
        temp = node;
        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }
    BST new_node = create_node();
    new_node->value = value;
    new_node->parent = temp;
    new_node->left = new_node->right = NULL;
    if (temp != NULL)
    {
        if (value < temp->value)
            temp->left = new_node;
        else
            temp->right = new_node;
        return root;
    }
    return new_node;
}

BST transplant(BST root, BST u, BST v)
{
    // 注意:如果v成为根节点,x需要将其parent改为NULL,否则在第一次将v改为根节点后将不会再进入第一个分支
    //printf("%d %d\n", u->parent, v);
    if (u->parent == NULL)
    {
        if (v != NULL)
            v->parent = NULL;
        return v;
    }
    if (u->parent->left == u)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != NULL)
        v->parent = u->parent;
    return root;
}

BST tree_delete(BST root, ItemType key)
{
    //如果root为空，则什么都不做,同时要小心重复释放内存,因为free后不会置指针为NULL
    if (root == NULL)
        return NULL;
    BST del_one = tree_search(root, key);
    /*  如果左子树为空，则右子树与待删节点父节点互指,因为左子树为空,
        所以只需保留右子树结构上移即可，原本就有的无需变无，无的需要变有，右子树为空同理  */
    if (del_one->left == NULL)
        root = transplant(root, del_one, del_one->right);
    else if (del_one->right == NULL)
        root = transplant(root, del_one, del_one->left);
    else
    {
        BST next = tree_successor(del_one);
        if (next != del_one->right)
        {
            root = transplant(root, next, next->right);
            next->right = del_one->right;
            next->right->parent = next;
        }
        root = transplant(root, del_one, next);
        next->left = del_one->left;
        del_one->left->parent = next;
    }
    free(del_one);
    //printf("%d\n", root);
    return root;
}

void preorder_tree_walker(BST node)
{
    if (node != NULL)
    {
        printf("%d\n", node->value);
        preorder_tree_walker(node->left);
        preorder_tree_walker(node->right);
    }
}
void inorder_tree_walker(BST node)
{
    if (node != NULL)
    {
        inorder_tree_walker(node->left);
        printf("%d\n", node->value);
        inorder_tree_walker(node->right);
    }
}
void postorder_tree_walker(BST node)
{
    if (node != NULL)
    {
        postorder_tree_walker(node->left);
        postorder_tree_walker(node->right);
        printf("%d\n", node->value);
    }
}