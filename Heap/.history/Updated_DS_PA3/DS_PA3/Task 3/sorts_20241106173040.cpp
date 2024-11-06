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

// Helper function to merge two sorted linked lists
shared_ptr<ListItem<long>> merge(shared_ptr<ListItem<long>> left, shared_ptr<ListItem<long>> right)
{
    // Create a dummy node to help with the merging process
    shared_ptr<ListItem<long>> dummy(new ListItem<long>(0));
    shared_ptr<ListItem<long>> tail = dummy;

    while (left != nullptr && right != nullptr)
    {
        if (left->value <= right->value)
        {
            tail->next = left;
            left->prev = tail;
            left = left->next;
        }
        else
        {
            tail->next = right;
            right->prev = tail;
            right = right->next;
        }
        tail = tail->next;
    }

    // If there are remaining elements in the left or right part, append them
    if (left != nullptr)
    {
        tail->next = left;
        left->prev = tail;
    }
    else if (right != nullptr)
    {
        tail->next = right;
        right->prev = tail;
    }

    return dummy->next;  // Return the next node after the dummy node (the head of the merged list)
}

// Helper function to split the linked list into two halves
void splitList(shared_ptr<ListItem<long>> head, shared_ptr<ListItem<long>>& left, shared_ptr<ListItem<long>>& right)
{
    shared_ptr<ListItem<long>> slow = head;
    shared_ptr<ListItem<long>> fast = head;

    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    left = head;
    right = slow->next;
    slow->next = nullptr;  // Split the list by setting the next of the slow pointer to null
}

// Merge Sort Implementation for Linked List
shared_ptr<ListItem<long>> mergeSort(shared_ptr<ListItem<long>> head)
{
    if (head == nullptr || head->next == nullptr)
    {
        return head;  // Base case: single element or empty list
    }

    shared_ptr<ListItem<long>> left, right;
    splitList(head, left, right);  // Split the list into two halves

    // Recursively sort the two halves
    left = mergeSort(left);
    right = mergeSort(right);

    // Merge the two sorted halves
    return merge(left, right);
}

// Merge Sort Implementation (Vector-Based)
vector<long> MergeSort(vector<long> nums)
{
    // Create a linked list from the input vector
    LinkedList<long> list;

    // Insert elements from the vector into the linked list
    for (long num : nums) {
        list.insertAtTail(num);  // Use insertAtTail to insert elements into the linked list
    }

    // Sort the linked list using merge sort
    shared_ptr<ListItem<long>> sortedHead = mergeSort(list.getHead());

    // Convert the sorted linked list back to a vector
    vector<long> sorted_nums;
    shared_ptr<ListItem<long>> current = sortedHead;  // Get the head of the sorted list

    while (current != nullptr) {
        sorted_nums.push_back(current->value);  // Insert each element into the vector
        current = current->next;  // Move to the next element in the list
    }

    return sorted_nums;
}

//=====================================================================================

#endif
