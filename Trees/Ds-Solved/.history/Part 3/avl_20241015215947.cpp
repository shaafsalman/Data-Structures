#include <iostream>
#include <memory>
#include <algorithm>
#include "avl.h"

// Constructor to initialize the AVL tree with or without balancing
template <class T, class S, class C>
AVL<T, S, C>::AVL(bool isAVL) 
{
    this->isAVL = isAVL;
    this->root = nullptr;
}

// Function to insert a node into the tree
template <class T, class S, class C>
void AVL<T, S, C>::insertNode(shared_ptr<node<T, S, C>> newNode) 
{
    if (isAVL) {
        root = insertAVLHelper(root, newNode); // AVL insert with balancing
    } else {
        root = insertBSTHelper(root, newNode); // Regular BST insert
    }
}

// Function to help with inserting nodes into the AVL tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertAVLHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode) {
    if (current == nullptr) {
        return newNode;
    }

    if (newNode->workExperience < current->workExperience) {
        current->left = insertAVLHelper(current->left, newNode);
    } 
    else if (newNode->workExperience > current->workExperience) {
        current->right = insertAVLHelper(current->right, newNode);
    } else {
        return current;
    }

    return balanceTree(current); // Balance the tree after insertion
}

// Function to help with inserting nodes into the BST (non-AVL mode)
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertBSTHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode) {
    if (current == nullptr) {
        return newNode;
    }

    if (newNode->workExperience < current->workExperience) {
        current->left = insertBSTHelper(current->left, newNode);
    } 
    else if (newNode->workExperience > current->workExperience) {
        current->right = insertBSTHelper(current->right, newNode);
    }

    return current; // No balancing needed for regular BST
}

// Function to get the height of a node
template <class T, class S, class C>
int AVL<T, S, C>::height(shared_ptr<node<T, S, C>> p)
{
    if (p == nullptr) {
        return 0;
    } else {
        return p->height;
    }
}

// Function to get the balance factor of a node (used in AVL mode)
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

// Function to balance the tree after insertion or deletion (only in AVL mode)
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::balanceTree(shared_ptr<node<T, S, C>> current) {
    if (current == nullptr) {
        return current;
    }

    current->height = std::max(height(current->left), height(current->right)) + 1;

    int balanceFactor = getBalanceFactor(current);

    // Left heavy
    if (balanceFactor > 1) {
        if (getBalanceFactor(current->left) < 0) {
            current->left = rotateLeft(current->left);
        }
        return rotateRight(current);
    }

    // Right heavy
    if (balanceFactor < -1) {
        if (getBalanceFactor(current->right) > 0) {
            current->right = rotateRight(current->right);
        }
        return rotateLeft(current);
    }

    return current;
}

// Function to search for a node by its key
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchNode(T k) {
    return searchHelper(root, k);
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

// Function to delete a node from the tree
template <class T, class S, class C>
void AVL<T, S, C>::deleteNode(T k) {
    if (isAVL) {
        root = deleteAVLHelper(root, k); // AVL delete with balancing
    } else {
        root = deleteBSTHelper(root, k); // Regular BST delete
    }
}

// Function to help with deleting a node from the AVL tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::deleteAVLHelper(shared_ptr<node<T, S, C>> current, T k) {
    if (current == nullptr) {
        return current;
    }

    if (k < current->workExperience) {
        current->left = deleteAVLHelper(current->left, k);
    } else if (k > current->workExperience) {
        current->right = deleteAVLHelper(current->right, k);
    } else {
        if (current->left == nullptr || current->right == nullptr) {
            return current->left ? current->left : current->right;
        } else {
            shared_ptr<node<T, S, C>> temp = current->left;
            while (temp->right != nullptr) {
                temp = temp->right;
            }
            current->workExperience = temp->workExperience;
            current->fullName = temp->fullName;
            current->gender = temp->gender;
            current->left = deleteAVLHelper(current->left, temp->workExperience);
        }
    }

    return balanceTree(current);
}

// Function to help with deleting a node from the BST (non-AVL mode)
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::deleteBSTHelper(shared_ptr<node<T, S, C>> current, T k) {
    if (current == nullptr) {
        return current;
    }

    if (k < current->workExperience) {
        current->left = deleteBSTHelper(current->left, k);
    } else if (k > current->workExperience) {
        current->right = deleteBSTHelper(current->right, k);
    } else {
        if (current->left == nullptr || current->right == nullptr) {
            return current->left ? current->left : current->right;
        } else {
            shared_ptr<node<T, S, C>> temp = current->left;
            while (temp->right != nullptr) {
                temp = temp->right;
            }
            current->workExperience = temp->workExperience;
            current->fullName = temp->fullName;
            current->gender = temp->gender;
            current->left = deleteBSTHelper(current->left, temp->workExperience);
        }
    }

    return current; // No balancing needed for regular BST
}

template <class T, class S, class C>
void AVL<T, S, C>::inOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result) {
    if (!root) return;  // If the root is null, return
    inOrderTraversal(root->left, result);   // Traverse left subtree
    result.push_back(root->workExperience); // Process the current node (store its workExperience)
    inOrderTraversal(root->right, result);  // Traverse right subtree
}

// Function to return the root of the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::getRoot() {
    return root;
}
