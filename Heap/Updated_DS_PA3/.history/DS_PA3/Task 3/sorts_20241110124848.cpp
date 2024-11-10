#ifndef __SORTS_CPP
#define __SORTS_CPP

#include "sorts.h"
#include "LinkedList.cpp"

//===========================Helper Function Definition================================

// Helper function to merge two sorted linked list halves
template <class T>
std::shared_ptr<ListItem<T>> merge(std::shared_ptr<ListItem<T>> left, std::shared_ptr<ListItem<T>> right) {
    if (!left) return right;
    if (!right) return left;

    if (left->value < right->value) {
        left->next = merge(left->next, right);
        if (left->next) left->next->prev = left;
        left->prev = nullptr;
        return left;
    } else {
        right->next = merge(left, right->next);
        if (right->next) right->next->prev = right;
        right->prev = nullptr;
        return right;
    }
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
    if (!head || !head->next) return head;

    // Split the list into two halves
    std::shared_ptr<ListItem<T>> second = split(head);

    // Recursively sort each half
    head = mergeSortLinkedList(head);
    second = mergeSortLinkedList(second);

    // Merge the sorted halves
    return merge(head, second);
}

//===========================Sorting Function Implementations===========================

// Insertion Sort Implementation (Task 1)
vector<long> InsertionSort(vector<long> nums) {
    for (int i = 1; i < nums.size(); i++) {
        long key = nums[i];
        int j = i - 1;

        // Move elements of nums[0..i-1], that are greater than key, to one position ahead
        while (j >= 0 && nums[j] > key) {
            nums[j + 1] = nums[j];
            j--;
        }
        nums[j + 1] = key;
    }
    return nums;
}

// Merge Sort Implementation (Task 2)
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
