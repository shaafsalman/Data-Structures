#ifndef __SORTS_CPP
#define __SORTS_CPP

#include "sorts.h"
#include "LinkedList.cpp"  // Ensure this is included for the linked list-based Merge Sort.

//===========================Sorting Function Implementations===========================
// Binary Search Helper Function
// Binary Search to find the insertion position

vector<long> InsertionSort(vector<long> nums) {
    for (int i = 1; i < nums.size(); i++) {
        long key = nums[i];
        int j = i - 1;

        // Shift elements to the right to make space for the key
        while (j >= 0 && nums[j] > key) {
            nums[j + 1] = nums[j];
            j--;
        }

        // Insert the key into its correct position
        nums[j + 1] = key;
    }
    return nums;
}









// Helper function to merge two sorted linked list halves
template <class T>
std::shared_ptr<ListItem<T>> merge(std::shared_ptr<ListItem<T>> left, std::shared_ptr<ListItem<T>> right) {
    std::shared_ptr<ListItem<T>> dummy(new ListItem<T>(0));
    std::shared_ptr<ListItem<T>> current = dummy;

    // Merge the lists by choosing the smaller node each time
    while (left && right) {
        if (left->value < right->value) {
            current->next = left;
            left->prev = current;  // Maintain backward link
            left = left->next;
        } else {
            current->next = right;
            right->prev = current;  // Maintain backward link
            right = right->next;
        }
        current = current->next;
    }

    // Attach any remaining nodes from left or right
    if (left) {
        current->next = left;
        left->prev = current;
    } else if (right) {
        current->next = right;
        right->prev = current;
    }

    // Return the merged list starting from the first real element (skip dummy)
    auto head = dummy->next;
    if (head) head->prev = nullptr;  // Ensure the head has no previous node
    return head;
}

// Helper function to split the linked list into two halves
template <class T>
std::shared_ptr<ListItem<T>> split(std::shared_ptr<ListItem<T>> head) {
    std::shared_ptr<ListItem<T>> fast = head, slow = head;
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    std::shared_ptr<ListItem<T>> temp = slow->next;
    slow->next = nullptr;
    return temp;
}

// Helper function for Merge Sort on linked list
template <class T>
std::shared_ptr<ListItem<T>> mergeSortLinkedList(std::shared_ptr<ListItem<T>> head) {
    // Base case: return if head is null or there's only one element
    if (!head || !head->next) return head;

    // Split the list into two halves
    std::shared_ptr<ListItem<T>> second = split(head);

    // Recursively sort each half
    head = mergeSortLinkedList(head);
    second = mergeSortLinkedList(second);

    // Merge the sorted halves
    return merge(head, second);
}

// Task 2: Merge Sort (Linked List-Based)
vector<long> MergeSort(vector<long> nums) {
    // Step 1: Create and populate a linked list from the input vector
    LinkedList<long> list;
    for (long num : nums) {
        list.insertAtTail(num);
    }

    // Step 2: Sort the linked list using merge sort
    std::shared_ptr<ListItem<long>> sortedHead = mergeSortLinkedList(list.getHead());

    // Step 3: Convert the sorted linked list back to a vector
    vector<long> sortedNums;
    auto current = sortedHead;
    while (current) {
        sortedNums.push_back(current->value);
        current = current->next;
    }

    return sortedNums;
}

#endif
