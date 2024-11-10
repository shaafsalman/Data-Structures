#ifndef __HEAP_CPP
#define __HEAP_CPP

#include "heap.h"
#include <memory>
#include <climits>
#include <stdexcept>
#include <iostream>

using namespace std;

MinHeap::MinHeap(int cap) : capacity(cap), heap_size(0)
{
    harr = shared_ptr<int[]>(new int[cap]);
}

int MinHeap::parent(int i)
{
    if (i <= 0 || i >= heap_size)
        throw out_of_range("Index out of range for parent calculation.");
    return (i - 1) / 2;
}

int MinHeap::left(int i)
{
    int l = 2 * i + 1;
    if (l >= heap_size)
        return -1; // Return -1 if left child is out of bounds
    return l;
}

int MinHeap::right(int i)
{
    int r = 2 * i + 2;
    if (r >= heap_size)
        return -1; // Return -1 if right child is out of bounds
    return r;
}

int MinHeap::getMin()
{
    if (heap_size <= 0)
        throw underflow_error("Heap is empty.");
    return harr[0];
}

void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        throw overflow_error("Heap overflow, cannot insert key.");
    }

    int i = heap_size++;
    harr[i] = k;

    // Fix the min-heap property if it is violated
    while (i != 0 && harr[parent(i)] > harr[i])
    {
        swap(harr[i], harr[parent(i)]);
        i = parent(i);
    }
}

int MinHeap::extractMin()
{
    if (heap_size <= 0)
        throw underflow_error("Heap underflow, cannot extract minimum.");
    
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }

    int root = harr[0];
    harr[0] = harr[--heap_size];
    minHeapify(0);

    return root;
}

void MinHeap::decreaseKey(int i, int new_val)
{
    if (i < 0 || i >= heap_size)
        throw out_of_range("Index out of range for decreaseKey.");
    
    harr[i] = new_val;
    while (i != 0 && harr[parent(i)] > harr[i])
    {
        swap(harr[i], harr[parent(i)]);
        i = parent(i);
    }
}

void MinHeap::deleteKey(int i)
{
    if (i < 0 || i >= heap_size)
        throw out_of_range("Index out of range for deleteKey.");

    decreaseKey(i, INT_MIN);
    extractMin();
}

void MinHeap::minHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;

    if (l != -1 && harr[l] < harr[i])
        smallest = l;
    if (r != -1 && harr[r] < harr[smallest])
        smallest = r;

    if (smallest != i)
    {
        swap(harr[i], harr[smallest]);
        minHeapify(smallest);
    }
}

shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

#endif
