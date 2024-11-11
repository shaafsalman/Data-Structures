#ifndef __SORTS_CPP
#define __SORTS_CPP

#include "sorts.h"
#include "LinkedList.cpp" 


//===========================Helper Function Definition================================
// Helper function to merge two sorted linked list halves
template <class T>
shared_ptr<ListItem<T>> merge(shared_ptr<ListItem<T>> left, shared_ptr<ListItem<T>> right) {
    shared_ptr<ListItem<T>> dummy(new ListItem<T>(0));
    shared_ptr<ListItem<T>> current = dummy;

    while (left && right) {
        if (left->value < right->value) {
            current->next = left;
            left->prev = current;  // backward link
            left = left->next;
        } else {
            current->next = right;
            right->prev = current;  // backward link
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



template <class T>
shared_ptr<ListItem<T>> split(shared_ptr<ListItem<T>> head)
{
    shared_ptr<ListItem<T>> fast = head, slow = head;
    while (fast->next && fast->next->next)
    {
        fast = fast->next->next;
        slow = slow->next;
    }
    shared_ptr<ListItem<T>> temp = slow->next;
    slow->next = nullptr;
    return temp;
}

template <class T>
shared_ptr<ListItem<T>> mergeSortLinkedList(shared_ptr<ListItem<T>> head) {
    if (!head || !head->next) return head;

    // Split into two halves
    shared_ptr<ListItem<T>> second = split(head);

    // Recursively sort each half
    head = mergeSortLinkedList(head);
    second = mergeSortLinkedList(second);

    // Merge the sorted halves
    return merge(head, second);
}


//===================================================================================== 

//===========================Sorting Function Implementations===========================
// Binary Search Helper Function
// Binary Search to find the insertion position

vector<long> InsertionSort(vector<long> nums) {
    int n = nums.size();
    
    // Using a gap sequence for insertion sorting
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            long key = nums[i];
            int j = i;
            
            // shift elements forward while they are greater than the key
            while (j >= gap && nums[j - gap] > key) {
                nums[j] = nums[j - gap];
                j -= gap;
            }
            nums[j] = key;
        }
    }

    return nums;
}
//=====================================================================================

vector<long> MergeSort(vector<long> nums) {

    // create and populate a linked list from the input vector
    LinkedList<long> list;
    for (long num : nums) {
        list.insertAtTail(num);
    }

    // sort the linked list
    shared_ptr<ListItem<long>> sortedHead = mergeSortLinkedList(list.getHead());

    // convert the sorted linked list back to a vector
    vector<long> sortedNums;
    auto current = sortedHead;
    while (current) {
        sortedNums.push_back(current->value);
        current = current->next;
    }

    return sortedNums;
}

//=====================================================================================



#endif
