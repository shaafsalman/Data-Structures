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
    // Allocate memory for the heap array
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
    // If the heap is empty, return INT_MAX (or any value indicating it's empty)
    if (heap_size <= 0)
        return INT_MAX;

    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }

    // Store the minimum value (root)
    int root = harr[0];

    // Replace the root with the last element
    harr[0] = harr[heap_size - 1];
    heap_size--;

    // Heapify the root to restore the heap property
    MinHeap::heapify(0);

    return root;
}

void MinHeap::decreaseKey(int i, int new_val)
{
    // Check if the new value is smaller than the current value
    if (new_val > harr[i])
        return;

    // Update the value at index i
    harr[i] = new_val;

    // Move the element at index i up to restore the heap property
    while (i > 0 && harr[parent(i)] > harr[i])
    {
        std::swap(harr[i], harr[parent(i)]);
        i = parent(i);
    }
}

int MinHeap::getMin()
{
    // Return the minimum element (root)
    return harr[0];
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
    harr[i] = k;

    // Move the new key up to restore the heap property
    while (i > 0 && harr[parent(i)] > harr[i])
    {
        std::swap(harr[i], harr[parent(i)]);
        i = parent(i);
    }
}

void MinHeap::heapify(int i)
{
    // Get the indices of the left and right children
    int l = left(i);
    int r = right(i);

    // Find the smallest among the root, left, and right children
    int smallest = i;
    if (l != -1 && harr[l] < harr[smallest])
        smallest = l;
    if (r != -1 && harr[r] < harr[smallest])
        smallest = r;

    // If the smallest is not the root, swap and recursively heapify the affected subtree
    if (smallest != i)
    {
        std::swap(harr[i], harr[smallest]);
        heapify(smallest);
    }
}

shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

#endif
