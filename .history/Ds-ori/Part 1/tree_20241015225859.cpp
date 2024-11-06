#include "tree.h"
#include <iostream>

using namespace std;

template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) {
    this->root = root;
}

template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key);
}

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

template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    auto parentNode = findKey(key);
    if (!parentNode || findKey(newNode->key)) return false;

    parentNode->children.push_back(newNode);
    return true;
}

template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    auto parentNode = findKey(key);
    if (!parentNode) return {};

    return parentNode->children;
}

template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root);
}

template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (!currNode) return -1;

    int height = 0;
    for (auto& child : currNode->children) {
        height = max(height, findHeightHelper(child) + 1);
    }
    
    return height;
}

template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    if (root && root->key == key) return false;

    return deleteLeafHelper(root, key) != nullptr;
}

template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) return nullptr;

    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) {
        if ((*it)->key == key) {
            if ((*it)->children.empty()) {
                cout << "Deleting leaf node with key: " << key << endl;
                currNode->children.erase(it);
                return currNode;
            } else {
                return currNode;
            }
        } else {
            auto result = deleteLeafHelper(*it, key);
            if (result != nullptr) return result;
        }
    }
    
    return currNode;
}

template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (currNode) {
        for (auto& child : currNode->children) {
            deleteTree(child);
        }
    }
    currNode.reset();
}

template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root;
}
