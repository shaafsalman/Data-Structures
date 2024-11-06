#include "tree.h"

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) {
    this->root = root;
}

// Find a node with the given key
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key);
}

// Helper function to find a node by key
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (currNode == nullptr) {
        return nullptr;
    }
    if (currNode->key == key) {
        return currNode;
    }
    for (auto& child : currNode->children) {
        shared_ptr<node<T, S>> result = findKeyHelper(child, key);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// Insert a new node as the child of a node with the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    shared_ptr<node<T, S>> parent = findKey(key);
    if (parent == nullptr) {
        return false;
    }
    for (auto& child : parent->children) {
        if (child->key == newNode->key) {
            return false;
        }
    }
    parent->children.push_back(newNode);
    return true;
}

// Return all the children of the node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    shared_ptr<node<T, S>> node = findKey(key);
    if (node == nullptr) {
        return {};
    }
    return node->children;
}

// Find the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root);
}

// Helper function to calculate the height of the tree
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (currNode == nullptr || currNode->children.empty()) {
        return 0;
    }
    int maxChildHeight = 0;
    for (auto& child : currNode->children) {
        int childHeight = findHeightHelper(child);
        if (childHeight > maxChildHeight) {
            maxChildHeight = childHeight;
        }
    }
    return maxChildHeight + 1;
}

// Delete a node if it is a leaf node
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    if (root->key == key) {
        return false;
    }
    shared_ptr<node<T, S>> result = deleteLeafHelper(root, key);
    return result != nullptr;
}

// Helper function to delete a leaf node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    for (int i = 0; i < currNode->children.size(); ++i) {
        if (currNode->children[i]->key == key && currNode->children[i]->children.empty()) {
            currNode->children.erase(currNode->children.begin() + i);
            return currNode;
        }
        shared_ptr<node<T, S>> result = deleteLeafHelper(currNode->children[i], key);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// Delete the entire tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (currNode == nullptr) {
        return;
    }
    for (auto& child : currNode->children) {
        deleteTree(child);
    }
    currNode->children.clear();
}

// Return the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root;
}
