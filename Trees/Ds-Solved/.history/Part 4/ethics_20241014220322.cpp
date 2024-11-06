#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include "../Part 3/avl.h"

using namespace std;

// This function calculates the maximum applicants that you can shortlist
template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root)
{
    if (root == nullptr) {
        return 0; // Height of an empty tree is 0
    }
    // Calculate height of the tree
    int height = 0;
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);
    
    while (!q.empty()) {
        int size = q.size();
        height++; // Increase height for each level
        for (int i = 0; i < size; i++) {
            auto current = q.front();
            q.pop();
            if (current->left) {
                q.push(current->left);
            }
            if (current->right) {
                q.push(current->right);
            }
        }
    }
    return height; // Maximum number of applicants to shortlist is the height of the tree
}

// This function returns shortlisted candidates on the right most path to leaf
template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    shared_ptr<node<T, S, C>> current = root;

    while (current != nullptr) {
        result.push_back(current->fullName); // Assuming fullName holds the candidate's name
        if (current->right != nullptr) {
            current = current->right; // Go down the right side
        } else {
            current = current->left; // If no right child, go left
        }
    }

    return result; // Return the rightmost path candidates
}

// This function returns shortlisted candidates in-order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    int maxShortlist = number_to_shortlist(root); // Get maximum number of candidates to shortlist
    inOrderHelper(root, result, maxShortlist);
    return result; // Return the in-order shortlisted candidates
}

template <class T, class S, class C>
void AVL<T, S, C>::inOrderHelper(shared_ptr<node<T, S, C>> node, vector<T>& result, int maxShortlist) {
    if (node == nullptr || result.size() >= maxShortlist) {
        return;
    }
    inOrderHelper(node->left, result, maxShortlist); // Traverse left
    if (result.size() < maxShortlist) {
        result.push_back(node->fullName); // Add current node's name
    }
    inOrderHelper(node->right, result, maxShortlist); // Traverse right
}

// This function returns shortlisted candidates in level order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    int maxShortlist = number_to_shortlist(root); // Get maximum number of candidates to shortlist
    if (root == nullptr) return result;

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty() && result.size() < maxShortlist) {
        auto current = q.front();
        q.pop();
        result.push_back(current->fullName); // Add current node's name

        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }

    return result; // Return the level-order shortlisted candidates
}

// This function calculates the bias in the tree
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root)
{
    vector<float> ratios;
    if (root == nullptr) return ratios;

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        // Calculate females and males in the left subtree
        int females = 0, males = 0;
        calculateGenderCount(current->left, females, males);
        
        // Calculate the ratio
        float ratio = (males == 0) ? (females == 0 ? 0 : -1) : static_cast<float>(females) / males;
        ratios.push_back(ratio); // Store the ratio

        // Push left and right children to the queue
        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }

    return ratios; // Return the ratios for bias analysis
}

template <class T, class S, class C>
void AVL<T, S, C>::calculateGenderCount(shared_ptr<node<T, S, C>> node, int& females, int& males) {
    if (node == nullptr) return;
    
    // Count genders based on the node's gender
    if (node->gender == 'F') { // Assuming 'F' indicates female
        females++;
    } else if (node->gender == 'M') { // Assuming 'M' indicates male
        males++;
    }

    // Recurse on left and right children
    calculateGenderCount(node->left, females, males);
    calculateGenderCount(node->right, females, males);
}
