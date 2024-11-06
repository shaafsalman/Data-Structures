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
    vector<float> result;

    if (!root) return result;  // Return empty vector if tree is empty

    // Level-order traversal using a queue
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int males = 0;  // Male count
        int females = 0;  // Female count

        // Check only the left subtree for gender counts
        if (current->left) {
            countGender(current->left, males, females);  // Helper function to count genders in left subtree
        }

        // Calculate the bias ratio
        float ratio;
        if (males == 0 && females == 0) {
            ratio = 0.0;  // No males and no females
        } else if (males == 0) {
            ratio = 1.0;  // Only females present
        } else {
            ratio = static_cast<float>(females) / males;  // Female to male ratio
        }

        // Debug output for current node's bias calculation
        cout << "Current Node: " << current->fullName 
             << " | Males: " << males << " | Females: " << females 
             << " | Ratio: " << ratio << endl;

        result.push_back(ratio);  // Store the ratio for the current node

        // Continue level-order traversal
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    // Final debug output
    cout << "Final Bias Ratios: ";
    for (float r : result) {
        cout << r << " ";
    }
    cout << endl;

    return result;  // Return the computed bias ratios
}

// Helper function to count males and females in the left subtree
template <class T, class S, class C>
void AVL<T, S, C>::countGender(shared_ptr<node<T, S, C>> subtreeRoot, int &males, int &females) {
    if (!subtreeRoot) return;

    if (subtreeRoot->gender == "M") {
        males++;
    } else if (subtreeRoot->gender == "F") {
        females++;
    }

    countGender(subtreeRoot->left, males, females);
    countGender(subtreeRoot->right, males, females);
}
