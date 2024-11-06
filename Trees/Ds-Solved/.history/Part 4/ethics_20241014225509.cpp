#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include "../Part 3/avl.h"
#include <functional>
// This function calculates the maximum number of applicants that you can shortlist
template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root)
{
    if (!root) return 0;

    int height = 0;
    shared_ptr<node<T, S, C>> current = root;

    // Calculate the height of the right-most path from root to leaf
    while (current) {
        height++;
        current = current->right;
    }

    return height;  // Number of nodes on the right-most path
}

// This function returns the shortlisted candidates on the right-most path to the leaf
template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root)
{
    vector<T> shortlistedCandidates;
    shared_ptr<node<T, S, C>> current = root;

    // Traverse the right-most path and collect candidates
    while (current) {
        shortlistedCandidates.push_back(current->workExperience);  // Change to desired key if needed
        current = current->right;
    }

    return shortlistedCandidates;  // List of candidates on the right-most path
}

// This function returns shortlisted candidates using in-order traversal
template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> inOrderCandidates;
    function<void(shared_ptr<node<T, S, C>>)> inOrderTraversal = [&](shared_ptr<node<T, S, C>> node) {
        if (!node) return;
        inOrderTraversal(node->left);
        inOrderCandidates.push_back(node->workExperience);  // Change to desired key if needed
        inOrderTraversal(node->right);
    };

    inOrderTraversal(root);

    // Limit the number of candidates to match the size of the right-most path
    int shortlist_size = number_to_shortlist(root);
    if (inOrderCandidates.size() > shortlist_size) {
        inOrderCandidates.resize(shortlist_size);
    }

    return inOrderCandidates;
}

// This function returns shortlisted candidates using level-order traversal
template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> levelOrderCandidates;
    if (!root) return levelOrderCandidates;

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    // Traverse the tree in level-order
    while (!q.empty()) {
        shared_ptr<node<T, S, C>> node = q.front();
        q.pop();
        levelOrderCandidates.push_back(node->workExperience);  // Change to desired key if needed

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    // Limit the number of candidates to match the size of the right-most path
    int shortlist_size = number_to_shortlist(root);
    if (levelOrderCandidates.size() > shortlist_size) {
        levelOrderCandidates.resize(shortlist_size);
    }

    return levelOrderCandidates;
}

// This function calculates the bias in the tree by computing the ratio of Females to Males
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root)
{
    vector<float> biasRatios;
    if (!root) return biasRatios;

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    // Traverse the tree in level-order to calculate gender bias ratios
    while (!q.empty()) {
        shared_ptr<node<T, S, C>> node = q.front();
        q.pop();

        int maleCount = 0, femaleCount = 0;

        // Helper function to count the genders in the left subtree
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

    return biasRatios;  // Vector of gender bias ratios in level-order
}
