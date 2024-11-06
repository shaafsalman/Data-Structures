#ifndef __SORTS_CPP
#define __SORTS_CPP
#include "sorts.h"
#include "LinkedList.cpp"

//===========================Helper Function Definition================================

// Insertion Sort Implementation (Array-Based)
vector<long> InsertionSort(vector<long> nums)
{
    // Traverse from the second element to the end
    for (int i = 1; i < nums.size(); i++)
    {
        long key = nums[i];
        int j = i - 1;
        
        // Move elements greater than key to one position ahead of their current position
        while (j >= 0 && nums[j] > key)
        {
            nums[j + 1] = nums[j];
            j--;
        }
        // Place the key after the element smaller than it
        nums[j + 1] = key;
    }

    return nums;
}

// Merge Sort Helper Functions using LinkedList
vector<long> MergeSort(vector<long> nums)
{
    // Create a linked list from the input vector
    LinkedList<long> list;

    // Insert elements from the vector into the linked list
    for (long num : nums) {
        list.insert(num);
    }

    // Sort the linked list using the merge sort
    list.sort();

    // Convert the sorted linked list back to a vector
    return list.toVector();
}

//=====================================================================================

#endif
