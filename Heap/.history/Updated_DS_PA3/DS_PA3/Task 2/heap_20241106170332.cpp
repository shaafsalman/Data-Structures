#ifndef __HEAP_CPP
#define __HEAP_CPP
#include "heap.h"
#include <memory>
#include <iostream>
#include <climits>

using namespace std;

// Constructor: Initializes the heap with a given capacity
MinHeap::MinHeap(int cap)
{
    capacity = cap;
    heap_size = 0;
    harr = shared_ptr<int>(new int[cap], std::default_delete<int[]>());
}

// Return index of the parent of a node at index i
int MinHeap::parent(int i)
{
    return (i - 1) / 2;
}

// Get index of left child of node at index i
int MinHeap::left(int i)
{
    return (2 * i) + 1;
}

// Get index of right child of node at index i
int MinHeap::right(int i)
{
    return (2 * i) + 2;
}

// Removes the minimum element (or root) from the min heap
int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX;
    if (heap_size == 1)
    {
        heap_size--;
        return harr.get()[0];
    }

    // Store the minimum value and remove it from the heap
    int root = harr.get()[0];
    harr.get()[0] = harr.get()[heap_size - 1];
    heap_size--;
    MinHeapify(0);

    return root;
}

// Decreases key value of key at index i to new_val
void MinHeap::decreaseKey(int i, int new_val)
{
    harr.get()[i] = new_val;
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

// Returns the minimum key (key at root) from the min heap
int MinHeap::getMin()
{
    return (heap_size <= 0) ? INT_MAX : harr.get()[0];
}

// Deletes a key stored at index i
void MinHeap::deleteKey(int i)
{
    decreaseKey(i, INT_MIN);
    extractMin();
}

// Inserts a new key 'k'
void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        cout << "Overflow: Could not insert key\n";
        return;
    }

    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr.get()[i] = k;

    // Fix the min heap property if violated
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

// Function to maintain min-heap property
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr.get()[l] < harr.get()[i])
        smallest = l;
    if (r < heap_size && harr.get()[r] < harr.get()[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(harr.get()[i], harr.get()[smallest]);
        MinHeapify(smallest);
    }
}

// Returns the heap array (for debugging)
shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

#endif
