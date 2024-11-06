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
    // Base case: If the array has more than one element
    if (nums.size() > 1)
    {
        int mid = nums.size() / 2;  // Find the middle point
        vector<long> left(nums.begin(), nums.begin() + mid);  // Left subarray
        vector<long> right(nums.begin() + mid, nums.end());  // Right subarray

        // Recursively divide the left and right subarrays
        left = MergeSort(left);
        right = MergeSort(right);

        // Merge the sorted subarrays
        merge(nums, 0, mid - 1, nums.size() - 1);
    }

    return nums;  // Return the sorted vector
}

//=====================================================================================

//==============================Helper Functions=======================================



void merge(vector<long>& nums, int left, int mid, int right)
{
    // Get the sizes of two subarrays to be merged
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    vector<long> leftArr(n1), rightArr(n2);

    // Copy data to temporary arrays leftArr[] and rightArr[]
    for (int i = 0; i < n1; i++)
        leftArr[i] = nums[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = nums[mid + 1 + j];

    // Merge the temporary arrays back into the original array nums[]
    int i = 0;  // Initial index of left subarray
    int j = 0;  // Initial index of right subarray
    int k = left;  // Initial index of merged subarray

    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            nums[k] = leftArr[i];
            i++;
        }
        else
        {
            nums[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements of leftArr[], if any
    while (i < n1)
    {
        nums[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy any remaining elements of rightArr[], if any
    while (j < n2)
    {
        nums[k] = rightArr[j];
        j++;
        k++;
    }
}
#endif