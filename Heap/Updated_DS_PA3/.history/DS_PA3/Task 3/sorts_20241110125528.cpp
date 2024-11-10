#ifndef __SORTS_CPP
#define __SORTS_CPP

#include "sorts.h"
#include "LinkedList.cpp"
#include <algorithm>  // For lower_bound
#include <vector>

//===========================Helper Function Definition================================

// Helper function to merge two sorted linked list halves
template <class T>
std::shared_ptr<ListItem<T>> merge(std::shared_ptr<ListItem<T>> left, std::shared_ptr<ListItem<T>> right) {
    std::shared_ptr<ListItem<T>> dummy(new ListItem<T>(0));
    std::shared_ptr<ListItem<T>> current = dummy;

    while (left && right) {
        if (left->value < right->value) {
            current->next = left;
            left->prev = current;
            left = left->next;
        } else {
            current->next = right;
            right->prev = current;
            right = right->next;
        }
        current = current->next;
    }

    if (left) {
        current->next = left;
        left->prev = current;
    } else if (right) {
        current->next = right;
        right->prev = current;
    }

    auto head = dummy->next;
    if (head) head->prev = nullptr;
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
    if (!head || !head->next) return head;

    std::shared_ptr<ListItem<T>> second = split(head);
    head = mergeSortLinkedList(head);
    second = mergeSortLinkedList(second);

    return merge(head, second);
}

//===========================Sorting Function Implementations===========================

// Optimized Insertion Sort Implementation (Task 1)
vector<long> InsertionSort(vector<long> nums) {
    // For smaller inputs, use Insertion Sort
    if (nums.size() < 1000) {
        for (int i = 1; i < nums.size(); i++) {
            long key = nums[i];
            int j = i - 1;

            // Use std::lower_bound to find the correct position for the key in the sorted part
            auto pos = std::lower_bound(nums.begin(), nums.begin() + i, key);

            // Move the elements to make space for the key
            for (int k = i - 1; k >= pos - nums.begin(); k--) {
                nums[k + 1] = nums[k];
            }

            // Place the key at the correct position
            *pos = key;
        }
    } else {
        // For larger inputs, use Merge Sort (for efficiency)
        return MergeSort(nums);
    }

    return nums;
}

// Merge Sort Implementation (Task 2)
vector<long> MergeSort(vector<long> nums) {
    LinkedList<long> list;
    for (long num : nums) {
        list.insertAtTail(num);
    }

    std::shared_ptr<ListItem<long>> sortedHead = mergeSortLinkedList(list.getHead());

    vector<long> sortedNums;
    auto current = sortedHead;
    while (current) {
        sortedNums.push_back(current->value);
        current = current->next;
    }

    return sortedNums;
}

#endif
