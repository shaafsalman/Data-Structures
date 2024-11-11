#ifndef __HEAP_CPP
#define __HEAP_CPP

#include "heap.h"
#include <memory>
#include <climits>

using namespace std;

// Function to ensure that the heap property is maintained for a given node in the heap
void minHeap(int* harr, int heap_size, int i);

MinHeap::MinHeap(int cap) : capacity(cap), heap_size(0)
{
    // Initialize the heap with the given capacity. We use a shared pointer to manage the heap array.
    harr = shared_ptr<int>(new int[cap], default_delete<int[]>());
}

// Function to get the index of the parent of a given node
int MinHeap::parent(int i)
{
    if (i <= 0 || i >= heap_size)  // If the node is the root, it has no parent
        return -1;
    return (i - 1) / 2;  // Parent index is calculated by this formula
}

// Function to get the index of the left child of a given node
int MinHeap::left(int i)
{
    int l = 2 * i + 1;  // Left child index calculation
    if (l >= heap_size)  // If left child is out of bounds
        return -1;
    return l;  // Return left child index
}

// Function to get the index of the right child of a given node
int MinHeap::right(int i)
{
    int r = 2 * i + 2;  // Right child index calculation
    if (r >= heap_size)  // If right child is out of bounds
        return -1;
    return r;  // Return right child index
}

// Function to get the minimum element (root) of the heap
int MinHeap::getMin()
{
    if (heap_size <= 0)  // If heap is empty, return a large value
        return INT_MAX;
    return harr.get()[0];  // Root element is the minimum
}

// Function to insert a new element into the heap
void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)  // If heap is full, cannot insert
        return;

    int i = heap_size;  // Start at the last position
    heap_size++;  // Increment heap size

    // Insert the new element at the last position
    harr.get()[i] = k;

    // Move the newly inserted element up to maintain the heap property
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        // Swap with the parent if it's smaller
        int temp = harr.get()[i];
        harr.get()[i] = harr.get()[parent(i)];
        harr.get()[parent(i)] = temp;
        i = parent(i);  // Move to the parent
    }
}

// Function to extract the minimum element (root) from the heap
int MinHeap::extractMin()
{
    if (heap_size <= 0)  // If heap is empty, return a large value
        return INT_MAX;

    if (heap_size == 1)  // If there’s only one element left, remove it
    {
        heap_size--;
        return harr.get()[0];
    }

    // Store the root (minimum element)
    int root = harr.get()[0];
    // Move the last element to the root
    harr.get()[0] = harr.get()[heap_size - 1];
    heap_size--;  // Decrease heap size

    // Restore the heap property by calling minHeap
    minHeap(harr.get(), heap_size, 0);

    return root;  // Return the previous root (minimum element)
}

// Function to decrease the value of a node at index i
void MinHeap::decreaseKey(int i, int new_val)
{
    if (i < 0 || i >= heap_size)  // Check if index is valid
        return;

    // Set the new value for the node
    harr.get()[i] = new_val;

    // Move the node up the heap to restore the heap property
    while (i != 0 && harr.get()[parent(i)] > harr.get()[i])
    {
        // Swap with the parent if the current value is smaller
        int temp = harr.get()[i];
        harr.get()[i] = harr.get()[parent(i)];
        harr.get()[parent(i)] = temp;
        i = parent(i);  // Move to the parent
    }
}

// Function to delete the node at index i by decreasing its key to INT_MIN and then extracting the minimum
void MinHeap::deleteKey(int i)
{
    if (i < 0 || i >= heap_size)  // Ensure the index is valid
        return;

    // Decrease the key to a very small value
    decreaseKey(i, INT_MIN);
    // Extract the minimum (which will be the deleted node)
    extractMin();
}

// Function to get a shared pointer to the heap array
shared_ptr<int> MinHeap::getHeap()
{
    return harr;
}

// Function to maintain the min-heap property at a given node
void minHeap(int* harr, int heap_size, int i)
{
    int l = 2 * i + 1;  // Left child index
    int r = 2 * i + 2;  // Right child index
    int smallest = i;   // Assume the smallest is the current node

    // If left child is smaller than the current node, update smallest
    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    
    // If right child is smaller than the smallest node, update smallest
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;

    // If the smallest is not the current node, swap and recursively heapify the affected subtree
    if (smallest != i)
    {
        int temp = harr[i];
        harr[i] = harr[smallest];
        harr[smallest] = temp;
        minHeap(harr, heap_size, smallest);  // Recurse to restore heap property
    }
}

#endif
