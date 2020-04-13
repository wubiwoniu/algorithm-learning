#include "max_heap.h"
MaxHeap createHeap(int Size) //创建大小为Size的堆，返回指针
{
    MaxHeap hp = (MaxHeap)malloc(sizeof(struct Heap));
    hp->data = (ItemType *)malloc(sizeof(ItemType) * (Size + 1));
    hp->size = Size;
    hp->count = 0;
    return hp;
}

void sink(MaxHeap hp, int k) //下沉，将小值一步步移入下一层
{
    while (2 * k <= hp->count)
    {
        int ptr = 2 * k;
        if (ptr < hp->count && hp->data[ptr] < hp->data[ptr + 1])
            ptr++;
        if (hp->data[k] >= hp->data[ptr])
            break;
        ItemType temp = hp->data[k];
        hp->data[k] = hp->data[ptr];
        hp->data[ptr] = temp;
        k = ptr;
    }
}

void swim(MaxHeap hp, int k) //将大值一步步移入上一层
{
    //比父节点大则上移，直至比父节点小
    while (hp->data[k] > hp->data[k / 2] && k > 1)
    {
        ItemType temp = hp->data[k];
        hp->data[k] = hp->data[k / 2];
        hp->data[k / 2] = temp;
        k /= 2;
    }
}

bool insert(MaxHeap hp, ItemType value) //将value插入堆
{
    if (hp->count >= hp->size)
        return false;
    (hp->count)++;
    hp->data[hp->count] = value;
    swim(hp, hp->count);
    return true;
}

ItemType deleteMax(MaxHeap hp) //删除hp[1]
{
    ItemType ans = hp->data[1];
    hp->data[1] = hp->data[hp->count];
    (hp->count)--;
    sink(hp, 1);
    return ans;
}

void HeapSort(MaxHeap hp) //将堆转换为有序数组
{
    for (int i = (hp->count) / 2; i >= 1; i--)
    {
        sink(hp, i);
    }
    while (hp->count > 1)
    {
        ItemType temp = hp->data[1];
        hp->data[1] = hp->data[hp->count];
        hp->data[hp->count] = temp;
        (hp->count)--;
        //printf("%d\n", hp->count);
        sink(hp, 1);
    }
}