#ifndef TREE_H
#define TREE_H

#include <memory>
#include <vector>
#include <algorithm> 
using namespace std; 
// For std::max

// Template definition for the node class
template <class T, class S>
class node {
public:
    T key;
    S data;
    std::vector<std::shared_ptr<node<T, S>>> children;
    
    node(T key, S data) : key(key), data(data) {}
};

// Template definition for the Tree class
template <class T, class S>
class Tree {
private:
    std::shared_ptr<node<T, S>> root;

public:
    // Constructor
    Tree(std::shared_ptr<node<T, S>> root = nullptr);

    // Get the root of the tree
    std::shared_ptr<node<T, S>> getRoot();

    // Find key-related functions
    std::shared_ptr<node<T, S>> findKey(T key);
    std::shared_ptr<node<T, S>> findKeyHelper(std::shared_ptr<node<T, S>> currNode, T key);

    // Insert child functionality
    bool insertChild(std::shared_ptr<node<T, S>> newNode, T key);

    // Get all children of a node
    std::vector<std::shared_ptr<node<T, S>>> getAllChildren(T key);

    // Find the height of the tree
    int findHeight();
    int findHeightHelper(std::shared_ptr<node<T, S>> currNode);

    // Delete operations
    void deleteTree(std::shared_ptr<node<T, S>> currNode);
    bool deleteLeaf(T key);
    std::shared_ptr<node<T, S>> deleteLeafHelper(std::shared_ptr<node<T, S>> currNode, T key);
};

#endif // TREE_H

