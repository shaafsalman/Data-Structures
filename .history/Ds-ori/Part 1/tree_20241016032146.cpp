#include "tree.h"

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) 
{
    this->root = root; 
}

//------------------------
// Key Searching Functions
//------------------------

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) 
{
    return findKeyHelper(root, key); 
}

// Helper function to find a key in the tree recursively
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) 
{
    if (!currNode) 
    {
        return nullptr; 
    }
    
    if (currNode->key == key) 
    {
        return currNode; 
    }

    for (auto& child : currNode->children) 
    {
        auto result = findKeyHelper(child, key);
        
        if (result) 
        {
            return result; 
        }
    }

    return nullptr; 
}

//------------------------
// Insertion Function
//------------------------

// This function inserts the given node as a child of the node with the specified key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) 
{
    auto parentNode = findKey(key); 
    
    if (!parentNode || findKey(newNode->key)) 
    {
        return false; 
    }

    parentNode->children.push_back(newNode); 
    
    return true; 
}

//------------------------
// Child Retrieval Function
//------------------------

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) 
{
    auto parentNode = findKey(key); 
    
    if (!parentNode) 
    {
        return {}; 
    }

    return parentNode->children; 
}

//------------------------
// Tree Height Function
//------------------------

// This function returns the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() 
{
    return findHeightHelper(root); 
}

// Helper function to find the height of the tree recursively
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) 
{
    if (!currNode) 
    {
        return -1; 
    }

    int height = 0; 
    
    for (auto& child : currNode->children) 
    {
        height = max(height, findHeightHelper(child) + 1); 
    }

    return height; 
}

//------------------------
// Deletion Functions
//------------------------

// This function deletes a node with the specified key, if it is a leaf node
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) 
{
    if (root == nullptr || root->key == key) 
    {
        return false; 
    }

    return deleteLeafHelper(root, key) != nullptr; 
}

// Helper function to delete a leaf node recursively
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) 
{
    if (!currNode) 
    {
        return nullptr; 
    }

    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) 
    {
        if ((*it)->key == key) 
        {
            if ((*it)->children.empty()) 
            {
                currNode->children.erase(it); 
                return currNode; 
            } 
            else 
            {
                return nullptr; 
            }
        }
    }

    for (auto& child : currNode->children) 
    {
        if (deleteLeafHelper(child, key) != nullptr) 
        {
            return currNode; 
        }
    }

    return nullptr; 
}

//------------------------
// Tree Management Functions
//------------------------

// This function deletes the entire tree starting from the given node
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) 
{
    if (currNode) 
    {
        for (auto& child : currNode->children) 
        {
            deleteTree(child); 
        }

        currNode.reset(); 
    }
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() 
{
    return root; 
}
