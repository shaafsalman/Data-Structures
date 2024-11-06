#ifndef __SORTS_CPP
#define __SORTS_CPP
#include "sorts.h"
#include "LinkedList.cpp"

//===========================Helper Function Definition================================

//=====================================================================================
vector<long> InsertionSort(vector<long> nums)
{
    // Traverse through 1 to n
    for (int i = 1; i < nums.size(); i++)
    {
        long key = nums[i];  // The current element to be inserted
        int j = i - 1;  // Initialize the index of the previous element

        // Move elements of nums[0..i-1], that are greater than key,
        // to one position ahead of their current position
        while (j >= 0 && nums[j] > key)
        {
            nums[j + 1] = nums[j];  // Shift element to the right
            j--;  // Move to the next element on the left
        }

        // Place the key at its correct position
        nums[j + 1] = key;
    }
    
    return nums;  // Return the sorted vector
}

//=====================================================================================
vector<long> MergeSort(vector<long> nums)
{
    // Add code here
}

//=====================================================================================

//==============================Helper Functions=======================================

#endif