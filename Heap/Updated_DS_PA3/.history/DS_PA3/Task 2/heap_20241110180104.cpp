#ifndef __HEAP_CPP
#define __HEAP_CPP

#include "heap.h"
#include <memory>
#include <climits>

using namespace std;

void minHeapify(int* harr, int heap_size, int i);

MinHeap::MinHeap(int cap) : capacity(cap), heap_size(0)
{
    harr = shared_ptr<int>(new int[cap], default_delete<int[]>());
}

int MinHeap::parent(int i)
{
    if (i <= 0 || i >= heap_size)
        return -1;
    return (i - 1) / 2;
}

int MinHeap::left(int i)
{
    int l = 2 * i + 1;
    if (l >= heap_size)
        return -1;
    return l;
}

int MinHeap::right(int i)
{
    int r = 2 * i + 2;
    if (r >= heap_size)
        return -1;
    return r;
}

int MinHeap::getMin()
{
    if (heap_size <= 0)
        return INT_MAX;
    return harr.get()[0];
}

void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
        return;

    int i = heap_size;
    heap_size++;
    harr.get()[i] = k;

    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        int temp = harr.get()[i];
        harr.get()[i] = harr.get()[parent(i)];
        harr.get()[parent(i)] = temp;
        i = parent(i);
    }
}

int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX;

    if (heap_size == 1)
    {
        heap_size--;
        return harr.get()[0];
    }

    int root = harr.get()[0];
    harr.get()[0] = harr.get()[heap_size - 1];
    heap_size--;
    minHeapify(harr.get(), heap_size, 0);

    return root;
}

void MinHeap::decreaseKey(int i, int new_val)
{
    if (i < 0 || i >= heap_size)
        return;

    harr.get()[i] = new_val;
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        int temp = harr.get()[i];
        harr.get()[i] = harr.get()[parent(i)];
        harr.get()[parent(i)] = temp;
        i = parent(i);
    }
}

void MinHeap::deleteKey(int i)
{
    if (i < 0 || i >= heap_size)
        return;

    decreaseKey(i, INT_MIN);
    extractMin();
}

shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

void minHeapify(int* harr, int heap_size, int i)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int smallest = i;

    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;

    if (smallest != i)
    {
        int temp = harr[i];
        harr[i] = harr[smallest];
        harr[smallest] = temp;
        minHeapify(harr, heap_size, smallest);
    }
}

#endif
