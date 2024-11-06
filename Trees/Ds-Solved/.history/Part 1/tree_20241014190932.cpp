#include "tree.h"

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) {
    this->root = root;
}

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key);
}

// Helper function to find a key in the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) return nullptr;

    if (currNode->key == key) {
        return currNode;
    }

    for (auto &child : currNode->children) {
        shared_ptr<node<T, S>> result = findKeyHelper(child, key);
        if (result) return result;
    }

    return nullptr;
}

// This function inserts the given node as a child of the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    shared_ptr<node<T, S>> parent = findKey(key);
    if (!parent) return false;

    for (const auto& child : parent->children) {
        if (child->key == newNode->key) return false;
    }

    parent->children.push_back(newNode);
    return true;
}

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    shared_ptr<node<T, S>> node = findKey(key);
    if (!node) return vector<shared_ptr<node<T, S>>>();

    return node->children;
}

// This function returns the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root);
}

// Helper function to find height of the tree
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (!currNode || currNode->children.empty()) return 0;

    int maxChildHeight = 0;
    for (const auto& child : currNode->children) {
        maxChildHeight = max(maxChildHeight, findHeightHelper(child));
    }

    return maxChildHeight + 1;
}

// This function deletes the node of a given key (iff it is a leaf node)
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    if (root->key == key) return false;

    shared_ptr<node<T, S>> result = deleteLeafHelper(root, key);
    return result != nullptr;
}

// Helper function to delete leaf node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) {
        if ((*it)->key == key && (*it)->children.empty()) {
            currNode->children.erase(it);
            return currNode;
        }
        shared_ptr<node<T, S>> result = deleteLeafHelper(*it, key);
        if (result) return result;
    }

    return nullptr;
}

// This function deletes the entire tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (!currNode) return;

    for (auto& child : currNode->children) {
        deleteTree(child);
    }

    currNode->children.clear();
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root;
}
