#include <iostream>
#include <queue>
#include <stack>

#include <memory>
#include "../Part 3/avl.h"


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

template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    while (root) {
        result.push_back(root->workExperience);  
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




template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root)
{
    vector<float> result;
    if (!root) return result;

    auto countGenders = [](shared_ptr<node<T, S, C>> subtree) -> pair<int, int> {
        int males = 0, females = 0;
        if (subtree) {
            stack<shared_ptr<node<T, S, C>>> s;
            s.push(subtree);
            while (!s.empty()) {
                auto current = s.top();
                s.pop();
                if (current->gender == "M") males++;
                else if (current->gender == "F") females++;
                if (current->left) s.push(current->left);
                if (current->right) s.push(current->right);
            }
        }
        return {males, females};
    };

    // Level-order traversal to calculate bias at each node
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        // Calculate bias only for left child nodes
        if (current->left) {
            auto [males, females] = countGenders(current->left);

            // Calculate the bias ratio
            float ratio = (males + females == 0) ? 0 : (males == 0 ? 1 : static_cast<float>(females) / (males + females));
            result.push_back(ratio);
        }

        // Enqueue both left and right children for further processing
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return result;
}
