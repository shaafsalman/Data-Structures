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
        list.insertAtTail(num);  // Use insertAtTail to insert elements into the linked list
    }

    // Sort the linked list using merge sort (Assuming the list class has a sort method implemented)
    list.sort();  // Assuming the sort method is available in LinkedList

    // Convert the sorted linked list back to a vector
    vector<long> sorted_nums;
    shared_ptr<ListItem<long>> current = list.getHead();  // Get the head of the list

    while (current != nullptr) {
        sorted_nums.push_back(current->value);  // Insert each element into the vector
        current = current->next;  // Move to the next element in the list
    }

    return sorted_nums;
}

//=====================================================================================

#endif
