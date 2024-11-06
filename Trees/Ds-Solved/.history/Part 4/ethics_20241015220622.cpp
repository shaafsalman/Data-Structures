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
#include <iostream>
#include <memory>
#include <vector>
#include <queue> // Include queue for level-order traversal
using namespace std;

// Assume other parts of AVL and node structures are defined above

template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    /*
    Bias ratio interpretation:
    - The ratio of females to males in the left subtree of each node.
    - A higher ratio indicates a potential bias against males in the automated screening system,
      while a lower ratio suggests females are being underrepresented.
    - This can indicate systematic bias in hiring practices based on gender differences in work experience.
    */
    
    vector<float> result; // Vector to store bias ratios
    if (!root) return result; // Return empty vector if tree is empty

    // Level-order traversal using a queue
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        // Count males and females in the left subtree
        int males = 0;
        int females = 0;

        // Only count if there's a left child
        if (current->left) {
            queue<shared_ptr<node<T, S, C>>> leftQueue;
            leftQueue.push(current->left);

            // Traverse the left subtree
            while (!leftQueue.empty()) {
                auto leftNode = leftQueue.front();
                leftQueue.pop();

                // Count male and female nodes
                if (leftNode->gender == "M") {
                    males++;
                } else if (leftNode->gender == "F") {
                    females++;
                }

                // Continue traversal for left and right children in the left subtree
                if (leftNode->left) leftQueue.push(leftNode->left);
                if (leftNode->right) leftQueue.push(leftNode->right);
            }
        }

        // Calculate the ratio of females to males
        float ratio;
        if (males == 0 && females == 0) {
            ratio = 0.0; // No males and no females
        } else if (males == 0) {
            ratio = 1.0; // Only females present
        } else {
            ratio = static_cast<float>(females) / males; // Female to male ratio
        }

        result.push_back(ratio); // Store the ratio for the current node

        // Continue level-order traversal for the entire tree
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return result; // Return the computed bias ratios
}
