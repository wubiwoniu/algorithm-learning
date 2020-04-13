#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "max_heap.h"

#define MaxSize 20

int main()
{
    MaxHeap hp = createHeap(MaxSize);
    ItemType list[10] = {4, 67, 12, -8, 5, 0, 21, 5, 7, 143};
    for (int i = 0; i < 10; i++)
    {
        insert(hp, list[i]);
    }
    /*for (int i = 1; i <= hp->count; i++)
        printf("%d \n", hp->data[i]);*/
    HeapSort(hp);
    /*for (int i = 1; i <= 10; i++)
        printf("%d ", deleteMax(hp));*/
    for (int i = 1; i <= 10; i++)
        printf("%d ", hp->data[i]);
    system("PAUSE");
}
