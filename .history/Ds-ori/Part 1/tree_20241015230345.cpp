#include "tree.h"
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

// Node structure definition
template <class T, class S>
struct node {
    T key;
    S value;
    vector<shared_ptr<node<T, S>>> children;

    node(T k, S v) : key(k), value(v) {}
};

// Tree class definition
template <class T, class S>
class Tree {
private:
    shared_ptr<node<T, S>> root;

    shared_ptr<node<T, S>> findKeyHelper(shared_ptr<node<T, S>> currNode, T key);
    int findHeightHelper(shared_ptr<node<T, S>> currNode);
    pair<shared_ptr<node<T, S>>, shared_ptr<node<T, S>>> findNodeAndParent(T key);
    void deleteTree(shared_ptr<node<T, S>> currNode);

public:
    Tree(shared_ptr<node<T, S>> root);
    shared_ptr<node<T, S>> findKey(T key);
    bool insertChild(shared_ptr<node<T, S>> newNode, T key);
    vector<shared_ptr<node<T, S>>> getAllChildren(T key);
    int findHeight();
    bool deleteLeaf(T key);
    shared_ptr<node<T, S>> getRoot();
};

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) {
    this->root = root;
}

// Search for a key in the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key);
}

// Helper function to search recursively
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) return nullptr;
    if (currNode->key == key) return currNode;

    for (auto& child : currNode->children) {
        auto result = findKeyHelper(child, key);
        if (result) return result;
    }

    return nullptr;
}

// Insert a new child node
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    auto parentNode = findKey(key);
    if (!parentNode || findKey(newNode->key)) return false;

    parentNode->children.push_back(newNode);
    return true;
}

// Get all children of a node
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    auto parentNode = findKey(key);
    if (!parentNode) return {};

    return parentNode->children;
}

// Find the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root);
}

// Helper function to find the height recursively
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (!currNode) return -1;

    int height = 0;
    for (auto& child : currNode->children) {
        height = max(height, findHeightHelper(child) + 1);
    }

    return height;
}

// Delete a leaf node
template <typename K, typename V>
bool Tree<K, V>::deleteLeaf(K key) {
    if (root == nullptr) return false; // Tree is empty

    // Helper function to find the node and its parent
    auto result = findNodeAndParent(key);
    if (result.first == nullptr || !result.first->children.empty()) {
        return false; // Node is not found or it's not a leaf
    }

    // Remove the node from its parent's children
    auto parent = result.second;
    if (parent != nullptr) {
        auto& siblings = parent->children;
        siblings.erase(remove_if(siblings.begin(), siblings.end(),
                                  [&](shared_ptr<node<K, V>> n) { return n->key == key; }),
                        siblings.end());
    } else { // Deleting the root node
        root = nullptr;
    }
    return true;
}

// Helper function to find a node and its parent
template <class T, class S>
pair<shared_ptr<node<T, S>>, shared_ptr<node<T, S>>> Tree<T, S>::findNodeAndParent(T key) {
    shared_ptr<node<T, S>> parent = nullptr;
    shared_ptr<node<T, S>> currNode = root;

    while (currNode) {
        if (currNode->key == key) {
            return { currNode, parent };
        }
        parent = currNode;
        // Search children for the key
        for (auto& child : currNode->children) {
            if (child->key == key) {
                return { child, currNode };
            }
        }
        // If not found, break the loop
        currNode = nullptr;
    }

    return { nullptr, nullptr }; // Key not found
}

// Delete the entire tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (currNode) {
        for (auto& child : currNode->children) {
            deleteTree(child);
        }
    }
    currNode.reset();
}

// Get the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root;
}
