#include "tree.h"

// You aren't allowed to edit these method declarations or declare global variables

// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root)
{
        this->root = root;
}

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key)
{
    return findKeyHelper(root, key);
}

// Helper function to find a key in the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key)
{
    if (currNode == nullptr) {
        return nullptr;
    }
    if (currNode->key == key) {
        return currNode;
    }
    for (int i = 0; i < currNode->children.size(); i++) {
        shared_ptr<node<T, S>> result = findKeyHelper(currNode->children[i], key);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// This function inserts the given node as a child of the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key)
{
     shared_ptr<node<T, S>> parent = findKey(key);
    if (parent == nullptr) {
        return false;
    }
    for (int i = 0; i < parent->children.size(); i++) {
        if (parent->children[i]->key == newNode->key) {
            return false;
        }
    }
    parent->children.push_back(newNode);
    return true;
}

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key)
{
     shared_ptr<node<T, S>> node = findKey(key);
    if (node == nullptr) {
        return vector<shared_ptr<node<T, S>>>();
    }
    return node->children;
}

// This function returns the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight()
{
}

// Helper function to find height of the tree
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode)
{
}

// This function deletes the node of a given key (iff it is a leaf node)
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key)
{
}

// Helper function to delete leaf node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key)
{
}

// This function deletes the tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode)
{
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot()
{
}
