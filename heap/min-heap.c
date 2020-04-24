#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//最小堆,节点v的子树的key都小于等于v.key
typedef struct Heap *MinHeap;
typedef int ItemType;
struct Heap
{
    ItemType *data;
    int size; //可放入最大容量
    int count;
};
MinHeap createHeap(int Size);
void sink(MinHeap hp, int k);
void swim(MinHeap hp, int k);
bool insert(MinHeap hp, ItemType value);
ItemType deleteMax(MinHeap hp);
void HeapSort(MinHeap hp);

int main()
{
    MinHeap hp = createHeap(15);
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

MinHeap createHeap(int Size) //创建大小为Size的堆，返回指针
{
    MinHeap hp = (MinHeap)malloc(sizeof(struct Heap));
    hp->data = (ItemType *)malloc(sizeof(ItemType) * (Size + 1));
    hp->size = Size;
    hp->count = 0;
    return hp;
}

void sink(MinHeap hp, int k) //下沉，将大值一步步移入下一层
{
    while (2 * k <= hp->count)
    {
        int ptr = 2 * k;
        //ptr始终指向最小的那个
        //设ptr的兄弟节点为ptrb,如果k.key大于ptr.key,则交换下移
        //因为有ptr.key<ptrb.key且k.key>ptr.key,可得ptr.key比ptrb与k的key都小,此时ptr.key上移不会影响二叉堆的性质
        //否则不变,因为有ptr.key>k.key,结合ptrb.key>ptr.key,可得k.key<ptrb.key
        if (ptr < hp->count && hp->data[ptr] > hp->data[ptr + 1])
            ptr++;
        if (hp->data[k] <= hp->data[ptr])
            break;
        ItemType temp = hp->data[k];
        hp->data[k] = hp->data[ptr];
        hp->data[ptr] = temp;
        k = ptr;
    }
}

void swim(MinHeap hp, int k) //将小值一步步移入上一层
{
    //比父节点小则上移，直至比父节点大
    while (hp->data[k] < hp->data[k / 2] && k > 1)
    {
        ItemType temp = hp->data[k];
        hp->data[k] = hp->data[k / 2];
        hp->data[k / 2] = temp;
        k /= 2;
    }
}

bool insert(MinHeap hp, ItemType value) //将value插入堆
{
    if (hp->count >= hp->size)
        return false;
    (hp->count)++;
    hp->data[hp->count] = value;
    swim(hp, hp->count);
    return true;
}

ItemType deleteMax(MinHeap hp) //删除hp[1]
{
    ItemType ans = hp->data[1];
    hp->data[1] = hp->data[hp->count];
    (hp->count)--;
    sink(hp, 1);
    return ans;
}

void HeapSort(MinHeap hp) //将堆转换为有序数组
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