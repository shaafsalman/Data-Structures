#include <iostream>
#include <memory>
#include <algorithm>
#include "avl.h"

// Constructor
template <class T, class S, class C>
AVL<T, S, C>::AVL(bool isAVL) : isAVL(isAVL), root(nullptr) {}

// Helper function to get the height of a node
template <class T, class S, class C>
int AVL<T, S, C>::height(shared_ptr<node<T, S, C>> p) {
    return p ? p->height : 0;
}

// Helper function to get the balance factor of a node
template <class T, class S, class C>
int AVL<T, S, C>::getBalanceFactor(shared_ptr<node<T, S, C>> p) {
    return p ? height(p->left) - height(p->right) : 0;
}

// Helper function to perform a right rotation
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateRight(shared_ptr<node<T, S, C>> y) {
    shared_ptr<node<T, S, C>> x = y->left;
    shared_ptr<node<T, S, C>> T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// Helper function to perform a left rotation
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateLeft(shared_ptr<node<T, S, C>> x) {
    shared_ptr<node<T, S, C>> y = x->right;
    shared_ptr<node<T, S, C>> T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

// Helper function to balance the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::balanceTree(shared_ptr<node<T, S, C>> current) {
    if (!current) return current;

    // Update height of the current node
    current->height = std::max(height(current->left), height(current->right)) + 1;

    // Get balance factor
    int balanceFactor = getBalanceFactor(current);

    // Left heavy
    if (balanceFactor > 1) {
        if (getBalanceFactor(current->left) < 0) {
            current->left = rotateLeft(current->left); // Left-right case
        }
        return rotateRight(current); // Left-left case
    }

    // Right heavy
    if (balanceFactor < -1) {
        if (getBalanceFactor(current->right) > 0) {
            current->right = rotateRight(current->right); // Right-left case
        }
        return rotateLeft(current); // Right-right case
    }

    return current;
}

// Helper function for insertion
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode) {
    if (!current) return newNode;

    if (newNode->workExperience < current->workExperience) {
        current->left = insertHelper(current->left, newNode);
    } else if (newNode->workExperience > current->workExperience) {
        current->right = insertHelper(current->right, newNode);
    } else {
        return current; // Duplicate keys not allowed
    }

    // Balance the tree
    return balanceTree(current);
}

// This function inserts a given node in the tree
template <class T, class S, class C>
void AVL<T, S, C>::insertNode(shared_ptr<node<T, S, C>> newNode) {
    root = insertHelper(root, newNode);
}

// Helper function for searching a node
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchHelper(shared_ptr<node<T, S, C>> current, T k) {
    if (!current || current->workExperience == k)
        return current;

    if (k < current->workExperience)
        return searchHelper(current->left, k);

    return searchHelper(current->right, k);
}

// This function searches a node in a tree by its key
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchNode(T k) {
    return searchHelper(root, k);
}

// Helper function for deletion
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::deleteHelper(shared_ptr<node<T, S, C>> current, T k) {
    if (!current) return current;

    if (k < current->workExperience) {
        current->left = deleteHelper(current->left, k);
    } else if (k > current->workExperience) {
        current->right = deleteHelper(current->right, k);
    } else {
        if (!current->left || !current->right) {
            shared_ptr<node<T, S, C>> temp = current->left ? current->left : current->right;
            return temp;
        } else {
            shared_ptr<node<T, S, C>> temp = current->left;
            while (temp->right != nullptr)
                temp = temp->right;
            current->workExperience = temp->workExperience;
            current->fullName = temp->fullName;
            current->gender = temp->gender;
            current->left = deleteHelper(current->left, temp->workExperience);
        }
    }

    return balanceTree(current);
}

// This function deletes a given node from the tree
template <class T, class S, class C>
void AVL<T, S, C>::deleteNode(T k) {
    root = deleteHelper(root, k);
}

// This function returns the root of the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::getRoot() {
    return root;
}
