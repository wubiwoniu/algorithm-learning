#ifndef _MAX_HEAP_H
#define _MAX_HEAP_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct Heap *MaxHeap;
typedef int ItemType;
struct Heap
{
    ItemType *data;
    int size; //可放入最大容量
    int count;
};
MaxHeap createHeap(int Size);
void sink(MaxHeap hp, int k);
void swim(MaxHeap hp, int k);
bool insert(MaxHeap hp, ItemType value);
ItemType deleteMax(MaxHeap hp);
void HeapSort(MaxHeap hp);
#endif