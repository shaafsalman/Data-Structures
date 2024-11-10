#ifndef __HEAP_CPP
#define __HEAP_CPP

#include "heap.h"
#include <memory>
#include <climits>
#include <iostream>

using namespace std;

void minHeapify(int* harr, int heap_size, int i);

MinHeap::MinHeap(int cap) : capacity(cap), heap_size(0)
{
    harr = shared_ptr<int>(new int[cap], default_delete<int[]>());
}

int MinHeap::parent(int i)
{
    return (i > 0 && i < heap_size) ? (i - 1) / 2 : -1;  // Return -1 for out-of-range
}

int MinHeap::left(int i)
{
    int l = 2 * i + 1;
    return (l < heap_size) ? l : -1;  // Return -1 if left child is out of range
}

int MinHeap::right(int i)
{
    int r = 2 * i + 2;
    return (r < heap_size) ? r : -1;  // Return -1 if right child is out of range
}

int MinHeap::getMin()
{
    if (heap_size <= 0)
    {
        cout << "Heap is empty." << endl;
        return INT_MAX;  // Use INT_MAX as an error indicator for empty heap
    }
    return harr.get()[0];
}

void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        cout << "Heap overflow, cannot insert key." << endl;
        return;
    }

    int i = heap_size++;
    harr.get()[i] = k;

    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

int MinHeap::extractMin()
{
    if (heap_size <= 0)
    {
        cout << "Heap underflow, cannot extract minimum." << endl;
        return INT_MAX;  // Use INT_MAX as an error indicator for empty heap
    }
    
    if (heap_size == 1)
    {
        heap_size--;
        return harr.get()[0];
    }

    int root = harr.get()[0];
    harr.get()[0] = harr.get()[--heap_size];
    minHeapify(harr.get(), heap_size, 0);

    return root;
}

void MinHeap::decreaseKey(int i, int new_val)
{
    if (i < 0 || i >= heap_size)
    {
        cout << "Invalid index for decreaseKey." << endl;
        return;
    }

    harr.get()[i] = new_val;
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

void MinHeap::deleteKey(int i)
{
    if (i < 0 || i >= heap_size)
    {
        cout << "Attempting to delete at index " << i << " (heap_size: " << heap_size << ") - Invalid index.\n";
        return;  // Return without doing anything if the index is out of range
    }

    cout << "deleteKey called with index " << i << " (heap_size: " << heap_size << ")\n";

    // Decrease the key to INT_MIN and extract the minimum
    decreaseKey(i, INT_MIN);
    extractMin();  // This reduces the heap_size by 1
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
        swap(harr[i], harr[smallest]);
        minHeapify(harr, heap_size, smallest);
    }
}

#endif
