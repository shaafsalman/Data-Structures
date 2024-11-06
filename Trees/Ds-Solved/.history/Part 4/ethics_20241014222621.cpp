#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <functional> // Include this header
#include "../Part 3/avl.h"

using namespace std;

// This function calculates the maximum applicants that you can shortlist
template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root) {
    if (!root) return 0; // Base case: empty tree
    int count = 0;

    // Recursive function to count nodes based on certain criteria (e.g., gender)
    function<void(shared_ptr<node<T, S, C>>)> countHelper = [&](shared_ptr<node<T, S, C>> node) {
        if (node) {
            // Criteria for shortlisting (can be modified as per requirement)
            if (node->gender == "Female") {
                count++;
            }
            countHelper(node->left);
            countHelper(node->right);
        }
    };

    countHelper(root);
    return count;  // Return count of shortlisted candidates
}

// This function returns shortlisted candidates on the rightmost path to the leaf
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

// This function returns shortlisted candidates in-order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    vector<T> temp;

    // In-order traversal helper function
    function<void(shared_ptr<node<T, S, C>>)> inOrderHelper = [&](shared_ptr<node<T, S, C>> node) {
        if (node) {
            inOrderHelper(node->left);
            temp.push_back(node->workExperience);
            inOrderHelper(node->right);
        }
    };

    inOrderHelper(root);
    int limit = number_to_shortlist(root); // Get the limit on number of shortlisted candidates

    // Use static_cast to avoid type mismatch in std::min
    for (size_t i = 0; i < min(static_cast<size_t>(limit), temp.size()); ++i) {
        result.push_back(temp[i]);
    }

    return result;
}

// This function returns shortlisted candidates in level order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    queue<shared_ptr<node<T, S, C>>> q;

    if (root) q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        result.push_back(current->workExperience); // Add the current node's work experience

        // Push left and right children to the queue
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    int limit = number_to_shortlist(root); // Get the limit on number of shortlisted candidates
    return vector<T>(result.begin(), result.begin() + min(static_cast<size_t>(limit), result.size())); // Return only up to the limit
}

// This function calculates the bias in the tree
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

        countHelper(current->left); // Count in left subtree
        float ratio = (maleCount > 0) ? static_cast<float>(femaleCount) / maleCount : 0; // Compute ratio
        ratios.push_back(ratio);

        // Add children to the queue for next level
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return ratios;
}
