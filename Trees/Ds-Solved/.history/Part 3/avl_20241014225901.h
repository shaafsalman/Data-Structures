#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include "../Part 3/avl.h" // Adjust the path as needed
using namespace std;

// Function to calculate the bias (gender ratio) in the tree
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    vector<float> ratios; // To store gender ratios
    int maleCount = 0, femaleCount = 0;

    // Lambda to count males and females in the subtree
    auto countGender = [&](shared_ptr<node<T, S, C>> subtree) {
        if (!subtree) return; // Base case: empty subtree
        // Count genders
        if (subtree->gender == "M") maleCount++;
        else if (subtree->gender == "F") femaleCount++;
        // Recursively count in the left and right subtrees
        countGender(subtree->left);
        countGender(subtree->right);
    };

    // Lambda to calculate and store the gender ratio
    function<void(shared_ptr<node<T, S, C>>)> calculateRatios =
        [&](shared_ptr<node<T, S, C>> node) {
            if (!node) return; // Base case: empty node

            // Reset counts
            maleCount = 0;
            femaleCount = 0;

            // Count genders in the current subtree
            countGender(node);

            // Calculate ratio and handle division by zero
            if (maleCount == 0) {
                ratios.push_back(0.0f); // No males
            } else if (femaleCount == 0) {
                ratios.push_back(1.0f); // No females
            } else {
                ratios.push_back(static_cast<float>(femaleCount) / maleCount);
            }

            // Continue to traverse to the left and right subtrees
            calculateRatios(node->left);
            calculateRatios(node->right);
        };

    // Start ratio calculation from the root
    calculateRatios(root);
    return ratios; // Return the calculated ratios
}

// Function to perform in-order traversal and return shortlisted candidates
template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result; // To store the in-order traversal results

    function<void(shared_ptr<node<T, S, C>>)> inOrderHelper = [&](shared_ptr<node<T, S, C>> node) {
        if (!node) return; // Base case: empty node
        inOrderHelper(node->left); // Visit left child
        result.push_back(node->workExperience); // Process current node
        inOrderHelper(node->right); // Visit right child
    };

    inOrderHelper(root); // Start in-order traversal from root
    return result; // Return the collected results
}

// Function to perform level-order traversal
template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result; // To store level-order results
    if (!root) return result; // Check for empty tree

    vector<shared_ptr<node<T, S, C>>> queue; // Queue for level-order traversal
    queue.push_back(root);

    while (!queue.empty()) {
        shared_ptr<node<T, S, C>> current = queue.front(); // Get the front node
        queue.erase(queue.begin()); // Remove it from the queue
        result.push_back(current->workExperience); // Process current node

        // Add children to the queue
        if (current->left) queue.push_back(current->left);
        if (current->right) queue.push_back(current->right);
    }

    return result; // Return collected results
}

// Function to get the rightmost path to leaf
template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root) {
    vector<T> result; // To store the rightmost path
    shared_ptr<node<T, S, C>> current = root;

    while (current) {
        result.push_back(current->workExperience); // Process current node
        current = current->right; // Move to the right child
    }

    return result; // Return the rightmost path
}

// Function to calculate the number of applicants to shortlist
template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root) {
    // Implement your logic here
    // This is just a placeholder
    return 0; // Modify this based on your logic
}
