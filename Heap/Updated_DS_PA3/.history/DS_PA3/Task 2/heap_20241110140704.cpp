#ifndef __HEAP_CPP
#define __HEAP_CPP

#include "heap.h"
#include <memory>
#include <climits>
#include <iostream>

// Constructor: Initializes the heap with the given capacity and allocates memory for the heap array.
MinHeap::MinHeap(int cap)
{
    capacity = cap;
    heap_size = 0;
    harr = std::shared_ptr<int>(new int[capacity], std::default_delete<int[]>());
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
        heap_size--;  // Reduce size
        return harr.get()[0];  // If there's only one element, return it.
    }

    // Store the minimum value (root).
    int root = harr.get()[0];
    // Replace root with the last element and reduce the heap size.
    harr.get()[0] = harr.get()[heap_size - 1];
    heap_size--;

    // Reorganize the heap to maintain the min-heap property.
    int i = 0;
    while (true)
    {
        int smallest = i;
        int l = left(i);
        int r = right(i);

        if (l < heap_size && harr.get()[l] < harr.get()[smallest])
            smallest = l;

        if (r < heap_size && harr.get()[r] < harr.get()[smallest])
            smallest = r;

        if (smallest == i)
            break;

        std::swap(harr.get()[i], harr.get()[smallest]);
        i = smallest;
    }

    // Debug: Print heap after extraction
    std::cout << "Heap after extractMin: ";
    for (int j = 0; j < heap_size; j++) {
        std::cout << harr.get()[j] << " ";
    }
    std::cout << std::endl;

    return root;
}









// Decreases the key value of the node at index i to new_val. If the new value is smaller, the function ensures the heap property is maintained.
void MinHeap::decreaseKey(int i, int new_val)
{
    if (new_val >= harr.get()[i]) {
        std::cout << "New value is larger than current value\n";
        return;
    }
    harr.get()[i] = new_val;

    // Bubble up the changed value to restore the heap property.
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
    // Decrease the value of the node to INT_MIN, which is smaller than any possible value
    decreaseKey(i, INT_MIN);

    // Call extractMin to remove the newly minimized node (which is now the root).
    extractMin();
}

// Inserts a new key 'k' into the heap. If the heap is full, it resizes dynamically.
void MinHeap::insertKey(int k)
{
    // If the heap is full, resize it
    if (heap_size == capacity)
    {
        capacity *= 2;  // Double the capacity (or choose a resizing factor)
        std::shared_ptr<int> new_heap(new int[capacity], std::default_delete<int[]>());
        for (int i = 0; i < heap_size; ++i)
        {
            new_heap.get()[i] = harr.get()[i];  // Copy old elements to the new heap
        }
        harr = new_heap;  // Point to the new heap
    }

    // Insert the new key at the end of the heap
    harr.get()[heap_size] = k;
    int i = heap_size++;

    // Bubble up the inserted key to restore heap property
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        std::swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

// Returns a shared pointer to the internal heap array, allowing external code to access the heap contents.
std::shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

#endif
