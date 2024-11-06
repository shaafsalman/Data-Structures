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
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    /*
    The bias function calculates the ratio of females to males in the left subtree of each node.
    These ratios are returned in the order of a level-order traversal (BFS) of the tree.

    Bias ratio interpretation:
    - A higher ratio (more females in the left subtree compared to males) indicates that females
      are less likely to be promoted through the shortlisting algorithm.
    - If males dominate the left subtree, females are underrepresented, indicating a bias.
    */
    
    vector<float> result;
    if (!root) return result;  // Return empty result if the tree is empty
    
    // Level-order traversal using a queue
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int males = 0, females = 0;

        // Check the left subtree and calculate the gender count
        if (current->left) {
            queue<shared_ptr<node<T, S, C>>> leftQueue;
            leftQueue.push(current->left);
            
            while (!leftQueue.empty()) {
                auto leftNode = leftQueue.front();
                leftQueue.pop();
                
                // Count male and female nodes
                if (leftNode->gender == "M") males++;
                else if (leftNode->gender == "F") females++;
                
                // Traverse the left and right children of the left subtree
                if (leftNode->left) leftQueue.push(leftNode->left);
                if (leftNode->right) leftQueue.push(leftNode->right);
            }
        }

        // Calculate the ratio of females to males
        float ratio = (males == 0) ? (females > 0 ? 1.0f : 0.0f) : (float)females / males;
        result.push_back(ratio);

        // Continue the level-order traversal
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return result;
}
