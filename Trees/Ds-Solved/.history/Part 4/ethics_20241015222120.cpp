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
// Bias calculation function: calculates the ratio of females to males in the left subtree
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<Node<T, S, C>> root) {
    vector<float> ratios;
    if (!root) return ratios;

    // Level-order traversal to get the order of nodes
    queue<shared_ptr<Node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        shared_ptr<Node<T, S, C>> current = q.front();
        q.pop();

        // Compute bias for the left subtree of the current node
        int males = 0, females = 0;

        // Count genders in the left subtree
        countGender(current->left, males, females);

        // Calculate the bias ratio
        float ratio = (males > 0) ? static_cast<float>(females) / males : (females == 0 ? 0 : static_cast<float>(males));
        ratios.push_back(ratio);

        // Add left and right children to the queue
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return ratios;
}

// In-order traversal to collect the nodes in the result vector
template <class T, class S, class C>
void AVL<T, S, C>::inOrderTraversal(shared_ptr<Node<T, S, C>> node, vector<T>& result) {
    if (!node) return;
    inOrderTraversal(node->left, result);
    result.push_back(node->workExperience);
    inOrderTraversal(node->right, result);
}

// Count the number of males and females in the subtree rooted at the given node
template <class T, class S, class C>
void AVL<T, S, C>::countGender(shared_ptr<Node<T, S, C>> node, int& males, int& females) {
    if (!node) return;

    // Count genders
    if (node->gender == 'M') {
        males++;
    } else if (node->gender == 'F') {
        females++;
    }

    // Recurse on left and right children
    countGender(node->left, males, females);
    countGender(node->right, males, females);
}