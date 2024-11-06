#include "tree.h"

// You aren't allowed to edit these method declarations or declare global variables

// Constructor
template <class T, class S>
Tree<T,S>::Tree(shared_ptr<node<T,S> > root) {
    this->root = root;
}

// This function finds a key in the tree and returns the respective node
template <class T, class S>
shared_ptr<node<T,S> > Tree<T,S>::findKey(T key) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->key == key) {
        return root;
    }
    return findKeyHelper(root, key);
}

// Helper function to find a key in the tree
template <class T, class S>
shared_ptr<node<T,S> > Tree<T,S>::findKeyHelper(shared_ptr<node<T,S> > currNode, T key) {
    for (auto child : currNode->children) {
        if (child->key == key) {
            return child;
        }
        auto result = findKeyHelper(child, key);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// This function inserts the given node as a child of the given key
template <class T, class S>
bool Tree<T,S>::insertChild(shared_ptr<node<T,S> > newNode, T key) {
    auto parent = findKey(key);
    auto checkUnique = findKey(newNode->key);
    if (parent == nullptr) {
        return false;
    }
    if (checkUnique != nullptr) {
        return false;
    }
    parent->children.push_back(newNode);
    return true;
}

// This function returns all the children of a node with the given key
template <class T, class S>
vector<shared_ptr<node<T,S> > > Tree<T,S>::getAllChildren(T key) {
    vector<shared_ptr<node<T,S> > > children;
    auto parent = findKey(key);
    if (parent == nullptr) {
        return children;
    }
    return parent->children;
}

// This function returns the height of the tree
template <class T, class S>
int Tree<T,S>::findHeight() {
    if (root == nullptr) {
        return 0;
    }
    if (root->children.size() == 0) {
        return 0;
    }
    int height = findHeightHelper(root);
    return height;
}

// Helper function to find height of the tree
template <class T, class S>
int Tree<T,S>::findHeightHelper(shared_ptr<node<T,S> > currNode) {
    int max = -1;
    for (auto child : currNode->children) {
        int height = findHeightHelper(child);
        if (height > max) {
            max = height;
        }
    }
    return max + 1;
}

// This function deletes the node of a given key (iff it is a leaf node)
template <class T, class S>
bool Tree<T,S>::deleteLeaf(T key) {
    if (root == nullptr) {
        return false;
    }
    if (root->key == key) {
        return false;
    }
    return deleteLeafHelper(root, key) != nullptr;
}

// Helper function to delete leaf node
template <class T, class S>
shared_ptr<node<T,S> > Tree<T,S>::deleteLeafHelper(shared_ptr<node<T,S> > currNode, T key) {
    for (auto child : currNode->children) {
        if (child->key == key) {
            if (child->children.size() == 0) {
                currNode->children.erase(remove(currNode->children.begin(), currNode->children.end(), child), currNode->children.end());
                return currNode;
            }
            return nullptr;
        }
        auto result = deleteLeafHelper(child, key);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// This function deletes the tree
template <class T, class S>
void Tree<T,S>::deleteTree(shared_ptr<node<T,S> > currNode) {
    if (currNode == nullptr) {
        return;
    }
    for (auto child : currNode->children) {
        deleteTree(child);
    }
    currNode->children.clear();
    currNode = nullptr;
}
// This function prints the whole tree from the root
template <class T, class S>
void Tree<T,S>::printTree() {
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
    }
    printTreeHelper(root);
    cout << endl;
}

// Helper function to print the tree
template <class T, class S>
void Tree<T,S>::printTreeHelper(shared_ptr<node<T,S> > currNode) {
    if (currNode == nullptr) {
        return;
    }
    cout << currNode->key << " ";
    for (auto child : currNode->children) {
        printTreeHelper(child);
    }
}

// This function returns the root of the tree
template <class T, class S>
shared_ptr<node<T,S> >  Tree<T,S>::getRoot() {
    return root;
}

