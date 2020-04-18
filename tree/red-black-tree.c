#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*  红黑树5个特性
    1、所有节点要么是红节点要么是黑节点
    2、根节点为黑节点
    3、叶节点均为黑节点
    4、红节点的子节点均为黑节点
    5、任意节点到叶节点的路径上的黑节点数相同(不包括自身)   */
typedef struct node *TNode;
typedef struct tree *RB_tree;
typedef int ItemType;
struct node
{
    int color; //1为红,0为黑
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
void levelorder_tree_walker(RB_tree tree, int size);
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
    //printf("nil = %d\n", tree->nil);
    int array[] = {0, 6, 192, -6, 41, 4, 1, 8, 5, 98, -19, 85, 12, 2, 7};
    for (int i = 0; i < 15; i++)
    {
        tree = tree_insert(tree, array[i]);
        //printf("%d\n", i);
    }
    preorder_tree_walker(tree, tree->root);
    inorder_tree_walker(tree, tree->root);

    for (int i = 0; i < 15; i++)
    {
        //int temp;
        //scanf("%d", &temp);
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
        //printf("node = %d\n",node);
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
    //对node进行左旋,设其右子树为rc则由rc代替node的位置,node成为rc的左子树,rc的左子树成为node的右子树
    //中间需要判断几次子树是否为T.nil,否则无法设置parent值
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
    //跟一般二叉搜索树一样,不过nil代替了null,一路向下找直至为nil,同时记录每次的父节点,
    //如果一开始root就为空,则代表记录的父节点为nil,设root为要插入的节点
    //同样的,由于可能要修改指针的值,结束后返回修改后的树的root
    //设插入节点颜色为红是为了防止改变结点黑高度,减小修复红黑结构的难度
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
    //修复红黑结构也可能会修改指针值(旋转),所以也要返回root
    //插入一个红节点可能会有以下情况
    //1、插入了一个红节点作为根节点
    //2、插入红节点的父节点也是红节点,违反了红黑树中红节点子节点不能是红节点的性质
    //其余情况均不会打破红黑性质
    //另外,如果插入的红节点违反了红节点子节点不能为红节点的性质,那么它是有不为nil的祖父节点的
    //因为插入前该树是严格的红黑树,根节点不会是红节点,故插入节点的父节点一定有一个黑父节点
    tree = tree_insert_fixup(tree, new_node);
    return tree;
}

RB_tree tree_insert_fixup(RB_tree tree, TNode node)
{
    //针对以上情况对红黑结构进行修复,同样的,因为旋转会改变指针值,故要返回root
    //当其父节点为红时就要一直进行结构修复
    while (node->parent->color == 1)
    {
        //判断分支是为了看对应的叔叔节点
        if (node->parent == node->parent->parent->left)
        {
            TNode prc = node->parent->parent->right;
            //prc为红时则将node->parent改为黑,prc->parent改为红,prc改为黑
            //把node->parent改为黑是为了解决与node的颜色冲突
            //但这改变了包含这一部分子树的节点的黑高,所以该prc->parent为红
            //但这一操作右减少了经过prc的路径上的一个黑节点,所以设prc为黑
            if (prc->color == 1)
            {
                node->parent->color = 0;
                prc->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
                //printf("case1   ");
            }
            //prc为黑,且node为右子树,将node->parent作为支点左旋
            //此时离开case2就会进入case3,因为原来在右侧的两个红节点被左旋至了左侧
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    left_rotate(tree, node);
                }
                //父节点设为黑色,父节点的父节点节点设为红色,同时右旋
                node->parent->color = 0;
                node->parent->parent->color = 1;
                right_rotate(tree, node->parent->parent);
            }
        }
        //与上正好相反
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
            //同理,右旋后左侧两个红节点进入右侧
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
    //为根节点直接设为根即可
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
    //如果删除后需要修复,那么原来被删除的一定是一个黑节点,因为删除了黑节点会改变节点的黑高度
    //此时经过node的简单路径黑高少1,所以我们假设node的颜色为"红(node自身的颜色)+黑(被删除节点的颜色)"或"黑+黑"
    //所以经过node的简单路径的黑高就跟原来一样了,此时只需要将这个黑色一步步向上移动就行
    //接下来懒得打了,以后补吧
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
    //删除黑节点可能会导致红黑性质被破坏,所以利用temp跟踪改变了红黑性质的节点,check为开始检查的节点
    //删除时分为直接删除与寻找后继插入删除两种,当要删除的节点最多只有一个子树时只需要删除
    //直接删除时如果被删除节点是黑色,则会影响红黑树的黑高度,所以temp为被删除节点,check设为被删除节点存在的那个子节点
    //后继插入删除则先找到后继节点,用后继节点代替被删除节点,同时将后继的颜色改为被删除节点的颜色
    //减小删除带来的影响,同时后继的右子树代替后继的位置,此时如果后继是黑色的,则会改变黑高度
    //所以temp设为后继节点,check设为后继的右子树
    //PS:后继没有左子树,通过反证假设x为y后继且有左子树,那么左子树一定存在一个节点z成为y后继,这与x是y后继矛盾
    //printf("root = %d\n",tree->root);
    TNode temp, check;
    TNode del_one = tree_search(tree, key);
    temp = del_one;
    int origin_color = del_one->color;
    //如果左右子树有一个为空直接接上即可
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
        temp = tree_successor(tree, del_one);
        origin_color = temp->color;
        check = temp->right;
        if (temp == del_one->right)
        {
            check->parent = temp;
        }
        else
        {
            transplant(tree, temp, temp->right);
            temp->right = del_one->right;
            temp->right->parent = temp;
        }
        //将后继temp移至待删除节点的位置,并改颜色
        transplant(tree, del_one, temp);
        temp->left = del_one->left;
        temp->left->parent = temp;
        temp->color = del_one->color;
    }
    free(del_one);
    //被删除节点是黑的,那么就要修复
    //printf("root = %d\n",tree->root);
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
    /*else
    {
        printf("nil\n");
    }*/
}
void inorder_tree_walker(RB_tree tree, TNode node)
{
    if (node != tree->nil)
    {
        inorder_tree_walker(tree, node->left);
        printf("%d  %d\n", node->value, node->color);
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

/*void levelorder_tree_walker(RBT root, int size)
{
    int cnt = -1;
    int i = 0;
    TNode *stack = (TNode)malloc(sizeof(TNode) * (size * 2 + 5));
    int front, rear;
    front = rear = 0;
    stack[0] = root;
    rear = 1;
    while (front != rear)
    {
        TNode ptr = stack[front];
        //不为叶节点则打印并压入子树信息
        if (ptr != nil)
        {
            printf("  key =% d,color = %d  ", ptr->value, ptr->color);
            stack[rear++] = ptr->left;
            stack[rear++] = ptr->right;
            cnt++;
            if (cnt == pow(2, i))
            {
                printf("\n");
                i++;
            }
        }
    }
}*/