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


// This function performs an in-order traversal of the tree and collects results in the result vector.
template <class T, class S, class C>
void AVL<T, S, C>::inOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result)
{
    if (!root) return;  // Base case: if the node is null, return

    // Traverse the left subtree
    inOrderTraversal(root->left, result);
    
    // Visit the current node
    result.push_back(root->workExperience);  // Collect work experience or other key data
    
    // Traverse the right subtree
    inOrderTraversal(root->right, result);
}


// Bias Function
// Description: This function calculates the ratio of females to males in the left subtree
// of each node. It returns a vector of these ratios in the same order as the level-order traversal.
// This helps in identifying gender-based bias due to work experience differences.
// Bias Function
// Description: This function calculates the ratio of females to males in the left subtree
// of each node. It returns a vector of these ratios in the same order as the level-order traversal.
// This helps in identifying gender-based bias due to work experience differences.

// Function to calculate the bias ratio of females to males in the left subtree of each node
// Function to calculate the bias ratio of females to males in the left subtree of each node
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root)
{
    vector<float> result;
    if (!root) return result;

    // Helper lambda to count genders in the subtree
    auto countGenders = [](shared_ptr<node<T, S, C>> subtree) -> pair<int, int> {
        if (!subtree) return {0, 0}; // Return zero males and females
        int males = 0, females = 0;
        queue<shared_ptr<node<T, S, C>>> q;
        q.push(subtree);
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            if (current->gender == "M") males++;  // Count males
            else if (current->gender == "F") females++;  // Count females
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        return {males, females};  // Return the count of males and females
    };

    // Level-order traversal to calculate bias at each node
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        // Count males and females in the left subtree
        auto [males, females] = countGenders(current->left);

        // Avoid division by zero:
        // - If there are no males, and females exist, return the number of females.
        // - If both males and females are zero, the ratio should be 0.
        float ratio;
        if (males == 0) {
            ratio = (females > 0) ? static_cast<float>(females) : 0;
        } else {
            ratio = static_cast<float>(females) / males;
        }

        result.push_back(ratio);

        // Continue with the level-order traversal
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return result;
}
