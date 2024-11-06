#include "Tree.h"
#include <iostream>
#include <memory>
#include <vector>
// Constructor
template <class T, class S>
Tree<T, S>::Tree(shared_ptr<node<T, S>> root) : root(root) {}

// Function to get the root of the tree
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::getRoot() {
    return root;
}

// findKey method: Finds the node with the given key
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKey(T key) {
    return findKeyHelper(root, key);
}

// findKeyHelper method: Helper function to find a node with the given key
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::findKeyHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) {
        return nullptr;
    }
    if (currNode->key == key) {
        return currNode;
    }
    for (auto& child : currNode->children) {
        shared_ptr<node<T, S>> result = findKeyHelper(child, key);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

// insertChild method: Inserts a new node as a child of the node with the given key
template <class T, class S>
bool Tree<T, S>::insertChild(shared_ptr<node<T, S>> newNode, T key) {
    shared_ptr<node<T, S>> parentNode = findKey(key);
    if (!parentNode) {
        return false; // Parent node not found
    }
    for (const auto& child : parentNode->children) {
        if (child->key == newNode->key) {
            return false; // Node with the same key already exists
        }
    }
    parentNode->children.push_back(newNode);
    return true;
}

// getAllChildren method: Returns all the children of the node with the given key
template <class T, class S>
vector<shared_ptr<node<T, S>>> Tree<T, S>::getAllChildren(T key) {
    shared_ptr<node<T, S>> parentNode = findKey(key);
    if (!parentNode) {
        return {}; // Node with the given key does not exist
    }
    return parentNode->children;
}

// findHeight method: Calculates the height of the tree
template <class T, class S>
int Tree<T, S>::findHeight() {
    return findHeightHelper(root);
}

// findHeightHelper method: Helper function to calculate the height of the tree
template <class T, class S>
int Tree<T, S>::findHeightHelper(shared_ptr<node<T, S>> currNode) {
    if (!currNode) {
        return -1; // Return -1 for null nodes
    }
    int maxHeight = -1;
    for (const auto& child : currNode->children) {
        maxHeight = max(maxHeight, findHeightHelper(child));
    }
    return maxHeight + 1;
}

// deleteTree method: Deletes the entire tree
template <class T, class S>
void Tree<T, S>::deleteTree(shared_ptr<node<T, S>> currNode) {
    if (!currNode) {
        return;
    }
    for (auto& child : currNode->children) {
        deleteTree(child);
    }
    currNode->children.clear();
}

// deleteLeaf method: Deletes a node with the given key if it is a leaf node
template <class T, class S>
bool Tree<T, S>::deleteLeaf(T key) {
    if (!root || root->key == key) {
        return false; // Do not delete the root node
    }
    return deleteLeafHelper(root, key) != nullptr;
}

// deleteLeafHelper method: Helper function to delete a leaf node
template <class T, class S>
shared_ptr<node<T, S>> Tree<T, S>::deleteLeafHelper(shared_ptr<node<T, S>> currNode, T key) {
    if (!currNode) {
        return nullptr;
    }
    for (auto it = currNode->children.begin(); it != currNode->children.end(); ++it) {
        if ((*it)->key == key && (*it)->children.empty()) {
            currNode->children.erase(it);
            return currNode;
        }
        shared_ptr<node<T, S>> result = deleteLeafHelper(*it, key);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

int main() {
    // Creating root node
    shared_ptr<node<int, string>> root = make_shared<node<int, string>>(1, "Root");
    Tree<int, string> myTree(root);

    // Insert nodes
    shared_ptr<node<int, string>> child1 = make_shared<node<int, string>>(2, "Child 1");
    shared_ptr<node<int, string>> child2 = make_shared<node<int, string>>(3, "Child 2");
    shared_ptr<node<int, string>> grandchild1 = make_shared<node<int, string>>(4, "Grandchild 1");
    
    // Insert child1 and child2 under root
    cout << "Inserting Child 1 under Root: " << (myTree.insertChild(child1, 1) ? "Success" : "Failure") << endl;
    cout << "Inserting Child 2 under Root: " << (myTree.insertChild(child2, 1) ? "Success" : "Failure") << endl;

    // Insert grandchild1 under child1
    cout << "Inserting Grandchild 1 under Child 1: " << (myTree.insertChild(grandchild1, 2) ? "Success" : "Failure") << endl;

    // Test getAllChildren
    cout << "Children of Root (1): ";
    auto childrenOfRoot = myTree.getAllChildren(1);
    for (const auto& child : childrenOfRoot) {
        cout << child->key << " ";
    }
    cout << endl;

    cout << "Children of Child 1 (2): ";
    auto childrenOfChild1 = myTree.getAllChildren(2);
    for (const auto& child : childrenOfChild1) {
        cout << child->key << " ";
    }
    cout << endl;

    // Test findKey
    shared_ptr<node<int, string>> foundNode = myTree.findKey(3);
    if (foundNode) {
        cout << "Found node with key 3: " << foundNode->value << endl;
    } else {
        cout << "Node with key 3 not found." << endl;
    }

    // Test findHeight
    cout << "Height of the tree: " << myTree.findHeight() << endl;

    // Test deleteLeaf
    cout << "Deleting leaf node 4 (Grandchild 1): " << (myTree.deleteLeaf(4) ? "Success" : "Failure") << endl;

    cout << "Trying to delete node 1 (Root): " << (myTree.deleteLeaf(1) ? "Success" : "Failure") << endl;

    // Verify after deletion
    cout << "Children of Child 1 (2) after deletion: ";
    childrenOfChild1 = myTree.getAllChildren(2);
    for (const auto& child : childrenOfChild1) {
        cout << child->key << " ";
    }
    cout << endl;

    // Test deleteTree
    cout << "Deleting the entire tree..." << endl;
    myTree.deleteTree(myTree.getRoot());

    // Check if the tree is empty
    cout << "Height of the tree after deletion: " << myTree.findHeight() << endl;

    return 0;
}