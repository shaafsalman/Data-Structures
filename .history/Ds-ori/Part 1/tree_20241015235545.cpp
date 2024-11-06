#include "tree.h"

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) {
    this->root = root; // Initialize the root of the tree
}

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key); // Use helper function to find the key
}

// Helper function to find a key in the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) return nullptr; // Base case: if current node is null, return null
    if (currNode->key == key) return currNode; // Found the key

    // Recursively search in children
    for (auto& child : currNode->children) {
        auto result = findKeyHelper(child, key);
        if (result) return result; // If found in children, return result
    }

    return nullptr; // Key not found
}

// This function inserts the given node as a child of the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    auto parentNode = findKey(key); // Find parent node
    if (!parentNode || findKey(newNode->key)) return false; // Check for valid parent and duplicate key

    parentNode->children.push_back(newNode); // Insert new node as child
    return true; // Insertion successful
}

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    auto parentNode = findKey(key); // Find the parent node
    if (!parentNode) return {}; // If parent not found, return empty vector

    return parentNode->children; // Return the children of the parent
}

// This function returns the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root); // Use helper function to find height
}

// Helper function to find height of the tree
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (!currNode) return -1; // Base case: if current node is null, height is -1

    int height = 0; // Initialize height
    for (auto& child : currNode->children) {
        height = max(height, findHeightHelper(child) + 1); // Calculate height recursively
    }

    return height; // Return the maximum height
}

// This function deletes the node of a given key (iff it is a leaf node)
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    if (root == nullptr) return false; // Tree is empty

    // Helper function to find the node and its parent
    auto result = deleteLeafHelper(root, key);
    return result != nullptr; // Return true if deletion was successful
}

// Helper function to delete leaf node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) return nullptr; // Base case: if current node is null

    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) {
        if ((*it)->key == key) { // Check if the current child matches the key
            if ((*it)->children.empty()) { // Check if it is a leaf node
                currNode->children.erase(it); // Delete the leaf node
                return currNode; // Return current node after deletion
            } else {
                cout << "Cannot delete node with key: " << key << " as it is not a leaf." << endl;
                return currNode; // Node is not a leaf, return current node
            }
        } else {
            auto result = deleteLeafHelper(*it, key); // Search in children
            if (result != nullptr) return result; // If found and deleted in children, return result
        }
    }

    return nullptr; // Key not found
}

// This function deletes the tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (currNode) {
        for (auto& child : currNode->children) {
            deleteTree(child); // Recursively delete all children
        }
        currNode.reset(); // Reset current node (optional as it's going out of scope)
    }
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root; // Return the root of the tree
}