#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// Assuming the node structure is already defined as shown previously.

template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    vector<float> ratios; // To store the ratios for each node
    int maleCount = 0, femaleCount = 0;

    // Lambda function to count males and females in the subtree
    auto countGender = [&](shared_ptr<node<T, S, C>> subtree) {
        if (!subtree) return; // Base case: empty subtree

        // Count the genders in the current node
        if (subtree->gender == "M") maleCount++;
        else if (subtree->gender == "F") femaleCount++;

        // Recursively count in the left and right subtrees
        countGender(subtree->left);
        countGender(subtree->right);
    };

    // Function to calculate and store the gender ratio at each node
    function<void(shared_ptr<node<T, S, C>>, shared_ptr<node<T, S, C>>)> calculateRatios =
        [&](shared_ptr<node<T, S, C>> node, shared_ptr<node<T, S, C>> parent) {
            if (!node) return; // Base case: empty node

            // Reset counts for the current node's subtree
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

            // Continue to traverse to the next node
            calculateRatios(node->left, node);
            calculateRatios(node->right, node);
        };

    // Start ratio calculation from the root
    calculateRatios(root, nullptr);
    return ratios; // Return the calculated ratios
}
