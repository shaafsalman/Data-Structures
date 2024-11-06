#include <iostream>
#include <memory>
#include <queue>
#include <vector>
#include "../Part 3/avl.h"

// This function calculates the maximum applicants that you can shortlist
template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root) {
    if (!root) return 0;

    // The number of applicants equals the height of the rightmost leaf
    int height = 0;
    shared_ptr<node<T, S, C>> current = root;

    while (current) {
        height++;
        current = current->right;  // Traverse to the rightmost node
    }

    return height;
}

// This function returns shortlisted candidates on the rightmost path to the leaf
template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root) {
    vector<T> shortlisted;

    shared_ptr<node<T, S, C>> current = root;
    while (current) {
        shortlisted.push_back(current->workExperience); // Add the work experience or any other key
        current = current->right;  // Move to the rightmost node
    }

    return shortlisted;
}

// This function returns shortlisted candidates in in-order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;

    function<void(shared_ptr<node<T, S, C>>)> inorderHelper = [&](shared_ptr<node<T, S, C>> node) {
        if (!node) return;
        inorderHelper(node->left);
        result.push_back(node->workExperience);  // Use the work experience or another key
        inorderHelper(node->right);
    };

    inorderHelper(root);

    // Limit the number of results to the same as the rightmost path
    int shortlist_size = number_to_shortlist(root);
    if (result.size() > shortlist_size) {
        result.resize(shortlist_size);  // Resize the result to the appropriate number
    }

    return result;
}

// This function returns shortlisted candidates in level-order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    if (!root) return result;

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        shared_ptr<node<T, S, C>> node = q.front();
        q.pop();
        result.push_back(node->workExperience);  // Add the work experience or another key

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    // Limit the result to the same number of nodes as the rightmost path
    int shortlist_size = number_to_shortlist(root);
    if (result.size() > shortlist_size) {
        result.resize(shortlist_size);
    }

    return result;
}

// This function calculates the bias in the tree
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    vector<float> biasRatios;
    if (!root) return biasRatios;

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        shared_ptr<node<T, S, C>> node = q.front();
        q.pop();

        int maleCount = 0, femaleCount = 0;

        // Helper function to count genders in the left subtree
        function<void(shared_ptr<node<T, S, C>>)> countGender = [&](shared_ptr<node<T, S, C>> subtree) {
            if (!subtree) return;
            if (subtree->gender == "M") maleCount++;
            else if (subtree->gender == "F") femaleCount++;
            countGender(subtree->left);
            countGender(subtree->right);
        };

        countGender(node->left);

        // Calculate the ratio of females to males in the left subtree
        float ratio = maleCount > 0 ? static_cast<float>(femaleCount) / maleCount : 0;
        biasRatios.push_back(ratio);
    }

    return biasRatios;
}
