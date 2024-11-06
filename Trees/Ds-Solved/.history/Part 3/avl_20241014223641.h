#include <memory>
#include <vector>
#include <cstdlib>
#include <string> // Include for std::string
using namespace std;

#ifndef __AVL_H
#define __AVL_H

// Node of the tree
template <class T, class S, class C>
struct node {
    S fullName;                // Full name of the applicant
    T workExperience;          // Work experience of the applicant
    string gender;             // Gender of the applicant
    shared_ptr<node> left;     // Left child pointer
    shared_ptr<node> right;    // Right child pointer
    int height;                // Height of the node

    // Constructor to initialize a new node
    node(T w, S n, C g) {
        this->fullName = n;
        this->workExperience = w;
        this->gender = g;
        left = nullptr;
        right = nullptr;
        height = 1; // New node is initially added at leaf
    }
};

// AVL Class (This will be used for both BST and AVL Tree implementation)
template <class T, class S, class C>
class AVL {
    shared_ptr<node<T, S, C>> root; // Root of the AVL tree
    bool isAVL;                     // Flag to indicate if the tree is AVL

public:
    // Constructor
    AVL(bool isAVLTree);

    // Public interface for node manipulation
    void insertNode(shared_ptr<node<T, S, C>> newNode);
    void deleteNode(T k);
    shared_ptr<node<T, S, C>> getRoot();
    shared_ptr<node<T, S, C>> searchNode(T k);

    // Functions for AVL tree operations
    int height(shared_ptr<node<T, S, C>> p);
    int number_to_shortlist(shared_ptr<node<T, S, C>> root);
    vector<T> right_most(shared_ptr<node<T, S, C>> root);
    vector<T> in_order(shared_ptr<node<T, S, C>> root);
    vector<T> level_order(shared_ptr<node<T, S, C>> root);
    vector<float> bias(shared_ptr<node<T, S, C>> root);

    // Declare helper functions here
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
