#ifndef __HEAP_CPP
#define __HEAP_CPP

#include "heap.h"
#include <memory>
#include <climits>
#include <stdexcept>

using namespace std;

// Forward declaration of minHeapify function
void minHeapify(int* harr, int heap_size, int i);

MinHeap::MinHeap(int cap) : capacity(cap), heap_size(0)
{
    // Allocate memory for the heap array
    harr = shared_ptr<int>(new int[cap], default_delete<int[]>());
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
    return (l < heap_size) ? l : -1; // Return -1 if left child is out of bounds
}

int MinHeap::right(int i)
{
    int r = 2 * i + 2;
    return (r < heap_size) ? r : -1; // Return -1 if right child is out of bounds
}

int MinHeap::getMin()
{
    if (heap_size <= 0)
        throw underflow_error("Heap is empty.");
    return harr.get()[0];
}

void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
        throw overflow_error("Heap overflow, cannot insert key.");

    int i = heap_size++;
    harr.get()[i] = k;

    // Bubble up to maintain the min-heap property
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        swap(harr.get()[i], harr.get()[parent(i)]);
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
        return harr.get()[0];
    }

    int root = harr.get()[0];
    harr.get()[0] = harr.get()[--heap_size];
    minHeapify(harr.get(), heap_size, 0);  // Call to standalone minHeapify

    return root;
}

void MinHeap::decreaseKey(int i, int new_val)
{
    if (i < 0 || i >= heap_size)
        throw out_of_range("Index out of range for decreaseKey.");

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
        throw out_of_range("Index out of range for deleteKey.");

    decreaseKey(i, INT_MIN);
    extractMin();
}

shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

// Standalone helper function for min-heapify
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
