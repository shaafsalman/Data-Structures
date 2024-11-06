#include "Tree.h"
#include <iostream>
#include <memory>
#include <vector>
// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) : root(root) {}

// Function to get the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root;
}

// findKey method: Finds the node with the given key
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key);
}

// findKeyHelper method: Helper function to find a node with the given key
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) {
        return nullptr;
    }
    if (currNode->key == key) {
        return currNode;
    }
    for (auto& child : currNode->children) {
        shared_ptr<node<T, S>> result = findKeyHelper(child, key);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

// insertChild method: Inserts a new node as a child of the node with the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    shared_ptr<node<T, S>> parentNode = findKey(key);
    if (!parentNode) {
        return false; // Parent node not found
    }
    for (const auto& child : parentNode->children) {
        if (child->key == newNode->key) {
            return false; // Node with the same key already exists
        }
    }
    parentNode->children.push_back(newNode);
    return true;
}

// getAllChildren method: Returns all the children of the node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    shared_ptr<node<T, S>> parentNode = findKey(key);
    if (!parentNode) {
        return {}; // Node with the given key does not exist
    }
    return parentNode->children;
}

// findHeight method: Calculates the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root);
}

// findHeightHelper method: Helper function to calculate the height of the tree
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (!currNode) {
        return -1; // Return -1 for null nodes
    }
    int maxHeight = -1;
    for (const auto& child : currNode->children) {
        maxHeight = max(maxHeight, findHeightHelper(child));
    }
    return maxHeight + 1;
}

// deleteTree method: Deletes the entire tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (!currNode) {
        return;
    }
    for (auto& child : currNode->children) {
        deleteTree(child);
    }
    currNode->children.clear();
}

// deleteLeaf method: Deletes a node with the given key if it is a leaf node
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    if (!root || root->key == key) {
        return false; // Do not delete the root node
    }
    return deleteLeafHelper(root, key) != nullptr;
}

// deleteLeafHelper method: Helper function to delete a leaf node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) {
        return nullptr;
    }
    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) {
        if ((*it)->key == key && (*it)->children.empty()) {
            currNode->children.erase(it);
            return currNode;
        }
        shared_ptr<node<T, S>> result = deleteLeafHelper(*it, key);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

