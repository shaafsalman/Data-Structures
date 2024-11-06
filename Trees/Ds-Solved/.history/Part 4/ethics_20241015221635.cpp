#include <iostream>
#include <queue>
#include <memory>
#include "../Part 3/avl.h"

// This function calculates the total number of applicants that can be shortlisted.
// It is the height of the tree along the right-most path.
template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root)
{
    if (!root) return 0;
    int height = 0;
    while (root) {
        height++;
        root = root->right;
    }
    return height;
}

// This function returns shortlisted candidates on the right-most path from root to leaf.
template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    while (root) {
        result.push_back(root->workExperience);  // Push work experience (or key) as required
        root = root->right;
    }
    return result;
}

// This function returns the in-order traversal of the tree but limits the number of candidates
// to the height of the right-most path.
template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    int limit = number_to_shortlist(root);  // Get the number of applicants to shortlist
    inOrderTraversal(root, result);

    // Return only up to the number of applicants that can be shortlisted
    if (result.size() > limit) {
        result.resize(limit);
    }
    return result;
}

// This function returns the level-order traversal of the tree but limits the number of candidates
// to the height of the right-most path.
template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    if (!root) return result;

    int limit = number_to_shortlist(root);  // Get the number of applicants to shortlist
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);
    while (!q.empty() && result.size() < limit) {
        auto current = q.front();
        q.pop();
        result.push_back(current->workExperience);  // Push work experience (or key)
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    return result;
}

// Bias Function
// Description: This function calculates the ratio of females to males in the left subtree
// of each node. It returns a vector of these ratios in the same order as the level-order traversal.
// This helps in identifying gender-based bias due to work experience differences.
vector<float> AVL<int, string, string>::bias(shared_ptr<node<int, string, string>> root) {
    vector<float> ratios;

    if (root == nullptr) {
        return ratios;
    }

    int males = 0, females = 0;

    // Count males and females in the left subtree
    auto left_bias = bias(root->left);
    for (float ratio : left_bias) {
        males += ratio.first; // Assume first is males count
        females += ratio.second; // Assume second is females count
    }

    // Count males and females in the right subtree
    auto right_bias = bias(root->right);
    for (float ratio : right_bias) {
        males += ratio.first;
        females += ratio.second;
    }

    // Count the current node's gender
    if (root->gender == "M") {
        males++;
    } else if (root->gender == "F") {
        females++;
    }

    // Calculate the bias ratio
    float ratio = (females == 0) ? (males > 0 ? static_cast<float>(males) : 0) : static_cast<float>(males) / females;
    ratios.push_back(ratio);

    cout << "Current Node: " << root->name << " | Males: " << males << " | Females: " << females << " | Ratio: " << ratio << endl;

    return ratios;
}
