#include <iostream>
#include <memory>
#include "avl.h"

// Constructor: Initializes the AVL tree
template <class T, class S, class C>
AVL<T, S, C>::AVL(bool isAVL) : isAVL(isAVL), root(nullptr)
{
}

// This function inserts a given node into the tree
template <class T, class S, class C>
void AVL<T, S, C>::insertNode(shared_ptr<node<T, S, C>> newNode)
{
    root = insertHelper(root, newNode);
    if (isAVL) {
        root = balanceTree(root);
    }
}

// Helper function for insertion
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode)
{
    if (!current) {
        return newNode; // Base case, insert newNode
    }
    
    if (newNode->workExperience < current->workExperience) {
        current->left = insertHelper(current->left, newNode);
    } else {
        current->right = insertHelper(current->right, newNode);
    }

    // Update the height of the current node
    current->height = 1 + std::max(height(current->left), height(current->right));

    return current;
}

// This function searches for a node in the tree by its key (workExperience)
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchNode(T k)
{
    return searchHelper(root, k);
}

// Helper function for searching
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchHelper(shared_ptr<node<T, S, C>> current, T k)
{
    if (!current || current->workExperience == k) {
        return current; // Found or NULL
    }
    
    if (k < current->workExperience) {
        return searchHelper(current->left, k);
    } else {
        return searchHelper(current->right, k);
    }
}

// This function deletes a node with the given key from the tree
template <class T, class S, class C>
void AVL<T, S, C>::deleteNode(T k)
{
    root = deleteHelper(root, k);
    if (isAVL) {
        root = balanceTree(root);
    }
}

// Helper function for deletion
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::deleteHelper(shared_ptr<node<T, S, C>> current, T k) {
    if (!current) return nullptr; // Node not found

    if (k < current->workExperience) {
        current->left = deleteHelper(current->left, k);
    } else if (k > current->workExperience) {
        current->right = deleteHelper(current->right, k);
    } else {
        // Node to delete found
        if (!current->left || !current->right) {
            return current->left ? current->left : current->right; // Return non-null child or nullptr
        } else {
            // Node with two children: Get predecessor (max of left subtree)
            shared_ptr<node<T, S, C>> temp = findMax(current->left);
            current->workExperience = temp->workExperience;
            current->fullName = temp->fullName;
            current->gender = temp->gender;
            current->left = deleteHelper(current->left, temp->workExperience);
        }
    }

    // Balance the tree after deletion
    if (!current) return current;

    current->height = 1 + max(height(current->left), height(current->right));

    return balanceTree(current); // Balance the tree after deletion
}

// Helper function to find the node with the maximum value (predecessor)
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::findMax(shared_ptr<node<T, S, C>> current)
{
    while (current->right) {
        current = current->right;
    }
    return current;
}

// This function returns the root of the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::getRoot()
{
    return root;
}

// This function calculates and returns the height of the tree
template <class T, class S, class C>
int AVL<T, S, C>::height(shared_ptr<node<T, S, C>> p)
{
    return p ? p->height : 0;
}

// Balances the tree by checking balance factors and performing rotations if needed
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::balanceTree(shared_ptr<node<T, S, C>> current) {
    if (!current) return current;

    // Update the height of the current node
    current->height = 1 + max(height(current->left), height(current->right));

    // Get the balance factor
    int balanceFactor = getBalanceFactor(current);

    // Left heavy
    if (balanceFactor > 1) {
        // Left-right case
        if (getBalanceFactor(current->left) < 0) {
            current->left = rotateLeft(current->left);
        }
        return rotateRight(current); // Left-left case
    }

    // Right heavy
    if (balanceFactor < -1) {
        // Right-left case
        if (getBalanceFactor(current->right) > 0) {
            current->right = rotateRight(current->right);
        }
        return rotateLeft(current); // Right-right case
    }

    return current;
}


// Get balance factor of a node
template <class T, class S, class C>
int AVL<T, S, C>::getBalanceFactor(shared_ptr<node<T, S, C>> current) {
    if (!current) return 0;
    return height(current->left) - height(current->right);
}


// Right rotation
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateRight(shared_ptr<node<T, S, C>> y) {
    shared_ptr<node<T, S, C>> x = y->left;
    shared_ptr<node<T, S, C>> T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    // Return new root
    return x;
}


// Left rotation
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateLeft(shared_ptr<node<T, S, C>> x) {
    shared_ptr<node<T, S, C>> y = x->right;
    shared_ptr<node<T, S, C>> T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    // Return new root
    return y;
}

