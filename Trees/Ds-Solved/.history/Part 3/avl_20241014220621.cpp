#include <iostream>
#include <memory>
#include <algorithm>
#include "avl.h"

// Constructor to initialize the AVL tree with or without balancing
template <class T, class S, class C>
AVL<T, S, C>::AVL(bool isAVL) {
    this->isAVL = isAVL;
    this->root = nullptr;
}

// Function to get the height of a node
template <class T, class S, class C>
int AVL<T, S, C>::height(shared_ptr<node<T, S, C>> p) {
    if (p == nullptr) {
        return 0;
    } else {
        return p->height;
    }
}

// Function to get the balance factor of a node
template <class T, class S, class C>
int AVL<T, S, C>::getBalanceFactor(shared_ptr<node<T, S, C>> p) {
    if (p == nullptr) {
        return 0;
    } else {
        return height(p->left) - height(p->right);
    }
}

// Function to perform a right rotation
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateRight(shared_ptr<node<T, S, C>> y) {
    shared_ptr<node<T, S, C>> x = y->left;
    shared_ptr<node<T, S, C>> T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

// Function to perform a left rotation
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateLeft(shared_ptr<node<T, S, C>> x) {
    shared_ptr<node<T, S, C>> y = x->right;
    shared_ptr<node<T, S, C>> T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

// Function to balance the tree after insertion or deletion
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::balanceTree(shared_ptr<node<T, S, C>> current) {
    if (current == nullptr) {
        return current;
    }

    current->height = std::max(height(current->left), height(current->right)) + 1;

    int balanceFactor = getBalanceFactor(current);

    if (balanceFactor > 1) {
        if (getBalanceFactor(current->left) < 0) {
            current->left = rotateLeft(current->left);
        }
        return rotateRight(current);
    }

    if (balanceFactor < -1) {
        if (getBalanceFactor(current->right) > 0) {
            current->right = rotateRight(current->right);
        }
        return rotateLeft(current);
    }

    return current;
}

// Function to help with inserting nodes into the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode) {
    if (current == nullptr) {
        return newNode;
    }

    if (newNode->workExperience < current->workExperience) {
        current->left = insertHelper(current->left, newNode);
    } else if (newNode->workExperience > current->workExperience) {
        current->right = insertHelper(current->right, newNode);
    } else {
        return current;
    }

    return balanceTree(current);
}

// Function to insert a node into the tree
template <class T, class S, class C>
void AVL<T, S, C>::insertNode(shared_ptr<node<T, S, C>> newNode) {
    root = insertHelper(root, newNode);
}

// Function to help with searching for a node in the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchHelper(shared_ptr<node<T, S, C>> current, T k) {
    if (current == nullptr || current->workExperience == k) {
        return current;
    }

    if (k < current->workExperience) {
        return searchHelper(current->left, k);
    }

    return searchHelper(current->right, k);
}

// Function to search for a node by its key
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchNode(T k) {
    return searchHelper(root, k);
}

// Function to help with deleting a node from the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::deleteHelper(shared_ptr<node<T, S, C>> current, T k) {
    if (current == nullptr) {
        return current;
    }

    if (k < current->workExperience) {
        current->left = deleteHelper(current->left, k);
    } else if (k > current->workExperience) {
        current->right = deleteHelper(current->right, k);
    } else {
        if (current->left == nullptr || current->right == nullptr) {
            shared_ptr<node<T, S, C>> temp = current->left ? current->left : current->right;
            return temp;
        } else {
            shared_ptr<node<T, S, C>> temp = current->left;
            while (temp->right != nullptr) {
                temp = temp->right;
            }
            current->workExperience = temp->workExperience;
            current->fullName = temp->fullName;
            current->gender = temp->gender;
            current->left = deleteHelper(current->left, temp->workExperience);
        }
    }

    return balanceTree(current);
}

// Function to delete a node from the tree
template <class T, class S, class C>
void AVL<T, S, C>::deleteNode(T k) {
    root = deleteHelper(root, k);
}

// Function to return the root of the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::getRoot() {
    return root;
}


