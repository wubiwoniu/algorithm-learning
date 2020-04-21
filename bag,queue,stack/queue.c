#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int ItemType;
typedef struct node *LNode;
typedef struct list *queue;
struct node
{
    ItemType data;
    LNode next;
};
struct list
{
    LNode head;
};

queue create_queue();
bool is_empty(queue Q);
queue enqueue(queue Q, ItemType value);
ItemType dequeue(queue Q);

int main()
{
    queue Q = create_queue();
    int array[] = {1, 6, 2, 5, 1, 0, 7, 4, 8, 1};
    for (int i = 0; i < 5; i++)
        Q = enqueue(Q, array[i]);
    for (int i = 0; i < 5; i++)
        printf("%d\n", dequeue(Q));
    for (int i = 5; i < 10; i++)
        Q = enqueue(Q, array[i]);
    for (int i = 0; i < 5; i++)
        printf("%d\n", dequeue(Q));
    system("PAUSE");
}

queue create_queue()
{
    queue Q = (queue)malloc(sizeof(struct list));
    Q->head = (LNode)malloc(sizeof(struct node));
    Q->head->next = NULL;
    return Q;
}

bool is_empty(queue Q)
{
    return Q->head->next == NULL ? true : false;
}

queue enqueue(queue Q, ItemType value)
{
    LNode node = (LNode)malloc(sizeof(struct node));
    node->data = value;
    LNode index = Q->head;
    while (index->next != NULL)
        index = index->next;
    node->next = index->next;
    index->next = node;
    return Q;
}

ItemType dequeue(queue Q)
{
    ItemType ans;
    LNode node = Q->head->next;
    if (node != NULL)
    {
        ans = node->data;
        Q->head->next = node->next;
        free(node);
        return ans;
    }
    return -1;
}