#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include "../Part 3/avl.h"

using namespace std;

template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root) {
    if (!root) return 0; // Base case: empty tree
    return 1 + number_to_shortlist(root->left) + number_to_shortlist(root->right); // Include left and right
}

template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    shared_ptr<node<T, S, C>> current = root;

    while (current) {
        result.push_back(current->workExperience); // Add the current node's work experience
        current = current->right; // Move to the right child
    }

    return result;
}

template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    int limit = number_to_shortlist(root); // Get the limit on number of shortlisted candidates
    vector<T> temp;

    function<void(shared_ptr<node<T, S, C>>)> inOrderHelper = [&](shared_ptr<node<T, S, C>> node) {
        if (node) {
            inOrderHelper(node->left);
            if (temp.size() < limit) { // Only add up to the limit
                temp.push_back(node->workExperience);
            }
            inOrderHelper(node->right);
        }
    };

    inOrderHelper(root);
    return temp;
}

template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    int limit = number_to_shortlist(root); // Get the limit on number of shortlisted candidates
    queue<shared_ptr<node<T, S, C>>> q;

    if (root) q.push(root);

    while (!q.empty() && result.size() < limit) {
        auto current = q.front();
        q.pop();
        result.push_back(current->workExperience); // Add the current node's work experience

        // Push left and right children to the queue
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return result;
}

template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    vector<float> ratios;
    queue<shared_ptr<node<T, S, C>>> q;

    if (root) q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int maleCount = 0;
        int femaleCount = 0;

        // Count males and females in the left subtree
        function<void(shared_ptr<node<T, S, C>>)> countHelper = [&](shared_ptr<node<T, S, C>> node) {
            if (node) {
                if (node->gender == "Male") {
                    maleCount++;
                } else if (node->gender == "Female") {
                    femaleCount++;
                }
                countHelper(node->left);
                countHelper(node->right);
            }
        };

        countHelper(current->left); // Count only in left subtree

        // Calculate ratio and avoid division by zero
        float ratio = (maleCount > 0) ? static_cast<float>(femaleCount) / maleCount : 0.0f;
        ratios.push_back(ratio);

        // Add children to the queue for next level
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return ratios;
}
