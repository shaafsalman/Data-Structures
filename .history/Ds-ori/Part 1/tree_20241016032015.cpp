#include "tree.h"

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) {
    this->root = root; // Initialize the root of the tree
}

//------------------------
// Key Searching Functions
//------------------------

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key); // Use helper function to find the key
}

// Helper function to find a key in the tree recursively
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) return nullptr; // Base case: if current node is null, return null
    if (currNode->key == key) return currNode; // Found the key

    // Recursively search in each child
    for (auto& child : currNode->children) {
        auto result = findKeyHelper(child, key);
        if (result) return result; // If found in children, return the result
    }

    return nullptr; // Key not found in this subtree
}

//------------------------
// Insertion Function
//------------------------

// This function inserts the given node as a child of the node with the specified key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    auto parentNode = findKey(key); // Find the parent node by key
    // Check for valid parent and duplicate key
    if (!parentNode || findKey(newNode->key)) return false;

    parentNode->children.push_back(newNode); // Insert the new node as a child
    return true; // Insertion successful
}

//------------------------
// Child Retrieval Function
//------------------------

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    auto parentNode = findKey(key); // Find the parent node
    if (!parentNode) return {}; // If parent not found, return an empty vector

    return parentNode->children; // Return the children of the parent
}

//------------------------
// Tree Height Function
//------------------------

// This function returns the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root); // Use helper function to find height
}

// Helper function to find the height of the tree recursively
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (!currNode) return -1; // Base case: if current node is null, height is -1

    int height = 0; // Initialize height
    for (auto& child : currNode->children) {
        height = max(height, findHeightHelper(child) + 1); // Calculate height recursively
    }

    return height; // Return the maximum height found
}

//------------------------
// Deletion Functions
//------------------------

// This function deletes a node with the specified key, if it is a leaf node
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    // Check if the tree is empty or if we are trying to delete the root
    if (root == nullptr || root->key == key) return false; // Do not delete root

    // Start the deletion process and return success status
    return deleteLeafHelper(root, key) != nullptr;
}

// Helper function to delete a leaf node recursively
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) return nullptr; // Base case: if current node is null

    // Iterate through children to find the key
    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) {
        if ((*it)->key == key) { // Key found
            if ((*it)->children.empty()) { // Check if it's a leaf node
                currNode->children.erase(it); // Delete the leaf node
                return currNode; // Return the parent node
            } else {
                return nullptr; // Node is not a leaf, deletion fails
            }
        }
    }

    // Recursively search in children
    for (auto& child : currNode->children) {
        if (deleteLeafHelper(child, key) != nullptr) {
            return currNode; // If the child was deleted, return the parent node
        }
    }

    return nullptr; // Key not found
}

//------------------------
// Tree Management Functions
//------------------------

// This function deletes the entire tree starting from the given node
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (currNode) {
        for (auto& child : currNode->children) {
            deleteTree(child); // Recursively delete all children
        }
        currNode.reset(); // Reset current node (optional, as it's going out of scope)
    }
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root; // Return the root of the tree
}