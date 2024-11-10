#ifndef __HEAP_CPP
#define __HEAP_CPP
#include "heap.h"
#include <memory>
#include <algorithm> // for std::swap

MinHeap::MinHeap(int cap)
{
    // Initialize heap size to 0 and set the capacity
    capacity = cap;
    heap_size = 0;
    // Allocate memory for the heap array using shared_ptr for a single element
    // We will reinterpret this as an array
    harr = std::shared_ptr<int>(new int[cap], std::default_delete<int[]>());
}

int MinHeap::parent(int i)
{
    // Check if the index is valid (greater than 0)
    if (i <= 0 || i >= heap_size)
        return -1; // Invalid index, no parent

    return (i - 1) / 2; // Formula for the parent index
}

int MinHeap::left(int i)
{
    // Formula for the left child index
    int left_index = 2 * i + 1;

    // Check if left child exists
    if (left_index >= heap_size)
        return -1; // No left child

    return left_index;
}

int MinHeap::right(int i)
{
    // Formula for the right child index
    int right_index = 2 * i + 2;

    // Check if right child exists
    if (right_index >= heap_size)
        return -1; // No right child

    return right_index;
}
int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX; // Return a large value if heap is empty

    if (heap_size == 1)
    {
        heap_size--;
        return harr.get()[0]; // Return the only element in the heap
    }

    // Store the minimum value (root)
    int root = harr.get()[0];

    // Replace root with the last element
    harr.get()[0] = harr.get()[heap_size - 1];
    heap_size--;  // Decrease heap size

    // Reheapify the root
    int i = 0;
    while (true)
    {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        // Find the smallest among root, left child, and right child
        if (l != -1 && harr.get()[l] < harr.get()[smallest])
            smallest = l;
        if (r != -1 && harr.get()[r] < harr.get()[smallest])
            smallest = r;

        // If the smallest is not the root, swap and continue heapifying
        if (smallest != i)
        {
            std::swap(harr.get()[i], harr.get()[smallest]);
            i = smallest;
        }
        else
        {
            break; // Heap property restored
        }
    }

    return root;
}

void MinHeap::decreaseKey(int i, int new_val)
{
    // Check if the new value is smaller than the current value
    if (new_val > harr.get()[i])
        return;

    // Update the value at index i
    harr.get()[i] = new_val;

    // Move the element at index i up to restore the heap property
    while (i > 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        std::swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

int MinHeap::getMin()
{
    // Return the minimum element (root)
    return harr.get()[0];  // Accessing the first element (root)
}

void MinHeap::deleteKey(int i)
{
    // Decrease the value at index i to negative infinity
    decreaseKey(i, INT_MIN);

    // Remove the element at index i
    extractMin();
}

void MinHeap::insertKey(int k)
{
    // Check if the heap is full
    if (heap_size == capacity)
        return; // or resize the heap if needed (not required here)

    // Insert the new key at the end of the heap
    heap_size++;
    int i = heap_size - 1;
    harr.get()[i] = k;  // Accessing the array

    // Move the new key up to restore the heap property
    while (i > 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        std::swap(harr.get()[i], harr.get()[parent(i)]);
        i = parent(i);
    }
}

shared_ptr<int> MinHeap::getHeap()
{
    return harr;  // Return the shared pointer to the heap array
}

#endif
