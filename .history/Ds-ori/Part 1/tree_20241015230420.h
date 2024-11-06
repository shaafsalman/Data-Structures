#ifndef __TREE_H
#define __TREE_H

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// Struct for nodes of the tree
template <class T, class S>
struct node {
    T key;  // Key of the node
    S value; // Value associated with the key
    vector<shared_ptr<node<T, S>>> children; // Vector of shared pointers to child nodes

    // Constructor for the node
    node(T key, S value) {
        this->key = key;
        this->value = value;
    }
};

// Tree class definition
template <class T, class S>
class Tree {
private:
    shared_ptr<node<T, S>> root; // Root node of the tree

    // Helper functions for various operations
    shared_ptr<node<T, S>> findKeyHelper(shared_ptr<node<T, S>>, T);
    int findHeightHelper(shared_ptr<node<T, S>>);
    shared_ptr<node<T, S>> deleteLeafHelper(shared_ptr<node<T, S>>, T);
    void deleteTree(shared_ptr<node<T, S>>);

public:
    // Constructor
    Tree(shared_ptr<node<T, S>>);
    
    // Public member functions
    shared_ptr<node<T, S>> findKey(T);
    bool insertChild(shared_ptr<node<T, S>>, T);
    vector<shared_ptr<node<T, S>>> getAllChildren(T);
    int findHeight();
    bool deleteLeaf(T);
    shared_ptr<node<T, S>> getRoot();
};

#endif // __TREE_H
