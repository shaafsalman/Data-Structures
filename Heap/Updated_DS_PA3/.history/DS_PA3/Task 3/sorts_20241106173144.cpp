#ifndef __SORTS_CPP
#define __SORTS_CPP
#include "sorts.h"
#include "LinkedList.cpp"

//===========================Helper Function Definition================================

// Insertion Sort Implementation
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

// Merge Sort Helper Functions
void Merge(vector<long>& nums, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    // Recursively split the array
    Merge(nums, left, mid);
    Merge(nums, mid + 1, right);

    // Merge the sorted halves
    vector<long> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    // Merge the two sorted sub-arrays into temp
    while (i <= mid && j <= right)
    {
        if (nums[i] <= nums[j])
        {
            temp[k++] = nums[i++];
        }
        else
        {
            temp[k++] = nums[j++];
        }
    }

    // If there are remaining elements in the left sub-array
    while (i <= mid)
    {
        temp[k++] = nums[i++];
    }

    // If there are remaining elements in the right sub-array
    while (j <= right)
    {
        temp[k++] = nums[j++];
    }

    // Copy the merged elements back to the original array
    for (int i = 0; i < temp.size(); i++)
    {
        nums[left + i] = temp[i];
    }
}

// Merge Sort Implementation
vector<long> MergeSort(vector<long> nums)
{
    Merge(nums, 0, nums.size() - 1);
    return nums;
}

//=====================================================================================

//==============================Helper Functions=======================================

#endif
