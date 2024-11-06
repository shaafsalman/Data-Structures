#ifndef __HEAP_CPP
#define __HEAP_CPP
#include "heap.h"
#include <memory>
#include <iostream>
#include <climits>

// Constructor: Initializes the heap with the given capacity and allocates memory for the heap array.
MinHeap::MinHeap(int cap)
{
    capacity = cap;
    heap_size = 0;
    // Allocate memory for the heap array
    harr = shared_ptr<int>(new int[capacity], std::default_delete<int[]>());
}

// Returns the index of the parent node of the node at index i.
int MinHeap::parent(int i)
{
    return (i - 1) / 2;
}

// Returns the index of the left child of the node at index i.
int MinHeap::left(int i)
{
    return 2 * i + 1;
}

// Returns the index of the right child of the node at index i.
int MinHeap::right(int i)
{
    return 2 * i + 2;
}

// Removes and returns the smallest element (root) of the heap. After removal, the heap is reorganized.
int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX; // If the heap is empty, return a large value.

    if (heap_size == 1)
    {
        heap_size--;
        return harr.get()[0]; // If there's only one element, return it.
    }

    // Store the minimum value (root).
    int root = harr.get()[0];
    // Move the last element to the root and decrease heap size.
    harr.get()[0] = harr.get()[heap_size - 1];
    heap_size--;

    // Reorganize the heap to maintain the min-heap property.
    MinHeapify(0);

    return root;
}

// Decreases the key value of the node at index i to new_val. If the new value is smaller, the function ensures the heap property is maintained.
void MinHeap::decreaseKey(int i, int new_val)
{
    // Change the value of the element.
    harr.get()[i] = new_val;

    // Ensure the heap property is maintained by "bubbling up" the element.
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        std::swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

// Returns the minimum key (key at root) from the min heap, without removing it.
int MinHeap::getMin()
{
    if (heap_size <= 0)
        return INT_MAX; // If the heap is empty, return a large value.
    return harr.get()[0];
}

// Deletes the element at index i by first decreasing its value to negative infinity, then calling extractMin() to remove it.
void MinHeap::deleteKey(int i)
{
    decreaseKey(i, INT_MIN);  // Set the key to negative infinity.
    extractMin();  // Remove the minimum element (which will be the one we decreased to negative infinity).
}

// Inserts a new key 'k' into the heap. If the heap is full, it resizes dynamically.
void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        std::cout << "Overflow: Could not insertKey\n";
        return;
    }

    // First insert the new key at the end of the heap.
    int i = heap_size++;
    harr.get()[i] = k;

    // Fix the min-heap property if it is violated by the new insertion.
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        std::swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

// Heapify a subtree with the root at given index (i) to maintain the min-heap property.
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;

    // Check if left child exists and is smaller than the current node.
    if (l < heap_size && harr.get()[l] < harr.get()[i])
        smallest = l;

    // Check if right child exists and is smaller than the current smallest.
    if (r < heap_size && harr.get()[r] < harr.get()[smallest])
        smallest = r;

    // If the smallest element is not the current node, swap and recursively heapify the affected subtree.
    if (smallest != i)
    {
        std::swap(harr.get()[i], harr.get()[smallest]);
        MinHeapify(smallest);
    }
}

// Returns a shared pointer to the internal heap array, allowing external code to access the heap contents.
shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

#endif
