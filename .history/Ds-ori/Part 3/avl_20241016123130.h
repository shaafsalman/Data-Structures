#include <memory>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;

#ifndef __AVL_H
#define __AVL_H

// Node of the tree
// The `node` structure holds the data for each individual node in the tree.
// It contains the full name, work experience, and gender, as well as pointers to the left and right children.
template <class T, class S, class C>
struct node
{
    S fullName;                // Stores the name of the employee
    T workExperience;          // Stores work experience (this is the key for comparison)
    C gender;                  // Stores the gender
    shared_ptr<node> left;     // Pointer to the left child
    shared_ptr<node> right;    // Pointer to the right child
    int height;                // Height of the node (used for AVL balancing)

    // Constructor to initialize the node with the given data
    node(T w, S n, C g)
    {
        this->fullName = n;
        this->workExperience = w;
        this->gender = g;
        left = nullptr;
        right = nullptr;
        height = 1;           // Height is 1 when the node is first created
    }
};

// AVL Class
// This class implements both AVL tree (balanced) and BST (non-balanced) based on the `isAVL` flag.
// When `isAVL` is true, it behaves as an AVL tree, otherwise as a regular Binary Search Tree (BST).
template <class T, class S, class C>
class AVL
{
    shared_ptr<node<T, S, C>> root; // Root node of the tree
    bool isAVL;                     // Determines if the tree is AVL (true) or BST (false)

public:
    // Constructor to initialize the AVL tree
    AVL(bool isAVL);

    // Basic AVL/BST operations
    // These functions will insert, delete, and search nodes in the tree
    void insertNode(shared_ptr<node<T, S, C>> newNode);  // Insert a node into the tree
    void deleteNode(T k);                               // Delete a node by work experience
    shared_ptr<node<T, S, C>> getRoot();                // Get the root of the tree
    shared_ptr<node<T, S, C>> searchNode(T k);          // Search for a node by work experience

    // Height function to return the height of a node
    int height(shared_ptr<node<T, S, C>> p);

    // Balance-related functions (for AVL tree)
    int getBalanceFactor(shared_ptr<node<T, S, C>> p);                 // Get balance factor of a node
    shared_ptr<node<T, S, C>> rotateRight(shared_ptr<node<T, S, C>> y); // Right rotation for balancing
    shared_ptr<node<T, S, C>> rotateLeft(shared_ptr<node<T, S, C>> x);  // Left rotation for balancing
    shared_ptr<node<T, S, C>> balanceTree(shared_ptr<node<T, S, C>> current); // Balance the tree (AVL only)

    // Insertion helpers (depending on isAVL)
    
    shared_ptr<node<T, S, C>> insertAVLHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode); // Insert for AVL
    shared_ptr<node<T, S, C>> insertBSTHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode); // Insert for BST

    // Deletion helpers (depending on isAVL)
    shared_ptr<node<T, S, C>> deleteAVLHelper(shared_ptr<node<T, S, C>> current, T k); // Delete for AVL
    shared_ptr<node<T, S, C>> deleteBSTHelper(shared_ptr<node<T, S, C>> current, T k); // Delete for BST

    // Search helper to find a node in the tree
    shared_ptr<node<T, S, C>> searchHelper(shared_ptr<node<T, S, C>> current, T k);

    // Helper functions for various traversals
    // These are utility functions to traverse the tree in different orders
    void inOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result);      // In-order traversal
    void levelOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result);   // Level-order traversal
    void rightMostTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result);    // Right-most traversal

    // Bias calculation function for statistical purposes
    void biasCalculation(shared_ptr<node<T, S, C>> root, vector<float>& result);

    // Part 4 Functions (Application-specific functionality)
    // These functions provide specific functionalities based on the project/assignment requirements.
    int number_to_shortlist(shared_ptr<node<T, S, C>> root); // Returns number of candidates to shortlist
    vector<T> right_most(shared_ptr<node<T, S, C>> root);    // Returns a vector with the right-most nodes
    vector<T> in_order(shared_ptr<node<T, S, C>> root);      // Returns a vector with nodes in in-order
    vector<T> level_order(shared_ptr<node<T, S, C>> root);   // Returns a vector with nodes in level-order
    vector<float> bias(shared_ptr<node<T, S, C>> root);      // Returns a vector with bias calculations
};

#endif