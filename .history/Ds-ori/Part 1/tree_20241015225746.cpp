#include "tree.h"

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root)
{
    this->root = root;  // Initialize the root of the tree
}

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key)
{
    return findKeyHelper(root, key);  // Call the helper function
}

// Helper function to find a key in the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key)
{
    if (!currNode) return nullptr;  // Return nullptr if the current node is null

    // Check if the current node's key matches the search key
    if (currNode->key == key)
        return currNode;

    // Recursively search in the children
    for (auto& child : currNode->children) {
        auto result = findKeyHelper(child, key);
        if (result) return result;  // If found, return the result
    }
    
    return nullptr;  // Return nullptr if not found
}

// This function inserts the given node as a child of the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key)
{
    auto parentNode = findKey(key);  // Find the parent node
    if (!parentNode || findKey(newNode->key)) return false;  // Fail if not found or key already exists

    parentNode->children.push_back(newNode);  // Add the new child to the parent's children
    return true;  // Insertion successful
}

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key)
{
    auto parentNode = findKey(key);  // Find the parent node
    if (!parentNode) return {};  // Return empty vector if not found

    return parentNode->children;  // Return the children of the found node
}

// This function returns the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight()
{
    return findHeightHelper(root);  // Call the helper function for height calculation
}

// Helper function to find height of the tree
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode)
{
    if (!currNode) return -1;  // Base case: null node height is -1

    int height = 0;  // Start with height of 0
    for (auto& child : currNode->children) {
        height = max(height, findHeightHelper(child) + 1);  // Recursively calculate height
    }
    
    return height;  // Return the maximum height
}

// This function deletes the node of a given key (iff it is a leaf node)
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key)
{
    // Cannot delete the root node if it is the only node
    if (root && root->key == key) return false;  

    return deleteLeafHelper(root, key) != nullptr;  // Attempt to delete the leaf
}

// Helper function to delete leaf node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key)
{
    if (!currNode) return nullptr;  // Base case for null node

    // Iterate through the children to find the key
    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) {
        if ((*it)->key == key) {
            // Check if it's a leaf node
            if ((*it)->children.empty()) {
                currNode->children.erase(it);  // Remove the leaf node from the parent's children
                return currNode;  // Return the current node after deletion
            } else {
                return currNode;  // Found the key but it's not a leaf node
            }
        } else {
            // Recursive call for children
            auto result = deleteLeafHelper(*it, key);
            if (result != nullptr) return result;  // If deleted, return the result
        }
    }
    
    return currNode;  // Return the current node if no deletion occurred
}

// This function deletes the tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode)
{
    if (currNode) {
        for (auto& child : currNode->children) {
            deleteTree(child);  // Recursively delete children
        }
    }
    currNode.reset();  // Clear the current node
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot()
{
    return root;  // Return the root node
}
