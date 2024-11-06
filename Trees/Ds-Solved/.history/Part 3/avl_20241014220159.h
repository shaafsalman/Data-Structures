#include <memory>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

#ifndef __AVL_H
#define __AVL_H

// Node of the tree
template <class T, class S, class C>
struct node
{
    S fullName;                 // Full name of the node
    T workExperience;           // Work experience of the node
    C gender;                   // Gender of the node
    shared_ptr<node<T, S, C>> left;  // Left child
    shared_ptr<node<T, S, C>> right; // Right child
    int height;                 // Height of the node

    // Node constructor
    node(T w, S n, C g) : workExperience(w), fullName(n), gender(g), left(nullptr), right(nullptr), height(1) {}
};

// AVL Class (This will be used for both BST and AVL Tree implementation)
template <class T, class S, class C>
class AVL
{
    shared_ptr<node<T, S, C>> root; // Root of the AVL tree
    bool isAVL;                     // Flag to indicate if it's an AVL tree

public:
    // Constructor
    AVL(bool isAVL);

    // Public interface for tree operations
    void insertNode(shared_ptr<node<T, S, C>> newNode);
    void deleteNode(T k);
    shared_ptr<node<T, S, C>> getRoot();
    shared_ptr<node<T, S, C>> searchNode(T k);
    int height(shared_ptr<node<T, S, C>> p);

    // Additional functions for tree traversal and analysis
    int number_to_shortlist(shared_ptr<node<T, S, C>> root);
    vector<T> right_most(shared_ptr<node<T, S, C>> root);
    vector<T> in_order(shared_ptr<node<T, S, C>> root);
    vector<T> level_order(shared_ptr<node<T, S, C>> root);
    vector<float> bias(shared_ptr<node<T, S, C>> root);

    // Declare helper functions
    shared_ptr<node<T, S, C>> insertHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode);
    shared_ptr<node<T, S, C>> deleteHelper(shared_ptr<node<T, S, C>> current, T k);
    shared_ptr<node<T, S, C>> searchHelper(shared_ptr<node<T, S, C>> current, T k);
    shared_ptr<node<T, S, C>> balanceTree(shared_ptr<node<T, S, C>> current);
    shared_ptr<node<T, S, C>> rotateRight(shared_ptr<node<T, S, C>> y);
    shared_ptr<node<T, S, C>> rotateLeft(shared_ptr<node<T, S, C>> x);
    shared_ptr<node<T, S, C>> findMax(shared_ptr<node<T, S, C>> current);
    int getBalanceFactor(shared_ptr<node<T, S, C>> current);
};

#endif // __AVL_H
