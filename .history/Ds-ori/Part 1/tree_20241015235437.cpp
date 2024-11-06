#include "tree.h"

// Constructor: Initializes the tree with a root node
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) : root(root) {}

// Helper function to find a key in the tree (recursive)
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (currNode == nullptr) {
        return nullptr;
    }
    if (currNode->key == key) {
        return currNode;
    }
    // Recursively search for the key in the children
    for (auto child : currNode->children) {
        auto result = findKeyHelper(child, key);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key);
}

// This function inserts the given node as a child of the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    auto parentNode = findKey(key);
    if (parentNode == nullptr) {  // If parent node doesn't exist
        return false;
    }

    // Ensure no child with the same key already exists
    for (auto child : parentNode->children) {
        if (child->key == newNode->key) {
            return false;
        }
    }

    // Add newNode as a child to parentNode
    parentNode->children.push_back(newNode);
    return true;
}

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    auto targetNode = findKey(key);
    if (targetNode != nullptr) {
        return targetNode->children;
    }
    return {};  // Return empty vector if key doesn't exist or has no children
}

// Helper function to find the height of the tree (recursive)
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (currNode == nullptr) {
        return -1;
    }

    int maxHeight = -1;
    for (auto child : currNode->children) {
        maxHeight = max(maxHeight, findHeightHelper(child));
    }
    return maxHeight + 1;
}

// This function returns the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root);
}

// This function deletes the node of a given key (iff it is a leaf node)
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    if (root == nullptr || root->key == key) {
        return false;  // Root can't be deleted or no node exists
    }
    auto nodeToDelete = findKey(key);
    if (nodeToDelete == nullptr || !nodeToDelete->children.empty()) {
        return false;  // Can't delete if it doesn't exist or isn't a leaf
    }

    // Find the parent of the node
    auto parent = deleteLeafHelper(root, key);
    if (parent != nullptr) {
        for (auto it = parent->children.begin(); it != parent->children.end(); ++it) {
            if ((*it)->key == key) {
                parent->children.erase(it);  // Erase the leaf node
                return true;
            }
        }
    }
    return false;
}

// Helper function to delete a leaf node (recursive)
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    for (auto child : currNode->children) {
        if (child->key == key) {
            return currNode;  // Return parent of the node to delete
        }
        auto parent = deleteLeafHelper(child, key);
        if (parent != nullptr) {
            return parent;
        }
    }
    return nullptr;
}

// This function deletes the entire tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (currNode != nullptr) {
        for (auto child : currNode->children) {
            deleteTree(child);
        }
        currNode->children.clear();
    }
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root;
}
