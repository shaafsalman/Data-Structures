#include <iostream>
#include <memory>
#include "avl.h"

template <class T, class S, class C>
AVL<T, S, C>::AVL(bool isAVL) : isAVL(isAVL), root(nullptr) {}

template <class T, class S, class C>
int getHeight(shared_ptr<node<T, S, C>> n) {
    return n ? n->height : 0;
}

template <class T, class S, class C>
void updateHeight(shared_ptr<node<T, S, C>> n) {
    n->height = 1 + std::max(getHeight(n->left), getHeight(n->right));
}

template <class T, class S, class C>
int getBalance(shared_ptr<node<T, S, C>> n) {
    return getHeight(n->left) - getHeight(n->right);
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> rightRotate(shared_ptr<node<T, S, C>> y) {
    shared_ptr<node<T, S, C>> x = y->left;
    shared_ptr<node<T, S, C>> T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> leftRotate(shared_ptr<node<T, S, C>> x) {
    shared_ptr<node<T, S, C>> y = x->right;
    shared_ptr<node<T, S, C>> T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

template <class T, class S, class C>
void AVL<T, S, C>::insertNode(shared_ptr<node<T, S, C>> newNode) {
    root = insertNode(root, newNode);
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> insertNode(shared_ptr<node<T, S, C>> node, shared_ptr<node<T, S, C>> newNode) {
    if (!node) return newNode;
    if (newNode->workExperience < node->workExperience) {
        node->left = insertNode(node->left, newNode);
    } else {
        node->right = insertNode(node->right, newNode);
    }
    updateHeight(node);
    if (isAVL) {
        int balance = getBalance(node);
        if (balance > 1 && newNode->workExperience < node->left->workExperience) {
            return rightRotate(node);
        }
        if (balance < -1 && newNode->workExperience > node->right->workExperience) {
            return leftRotate(node);
        }
        if (balance > 1 && newNode->workExperience > node->left->workExperience) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && newNode->workExperience < node->right->workExperience) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }
    return node;
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchNode(T k) {
    return searchNode(root, k);
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> searchNode(shared_ptr<node<T, S, C>> node, T k) {
    if (!node || node->workExperience == k) return node;
    if (k < node->workExperience) return searchNode(node->left, k);
    return searchNode(node->right, k);
}

template <class T, class S, class C>
void AVL<T, S, C>::deleteNode(T k) {
    root = deleteNode(root, k);
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> deleteNode(shared_ptr<node<T, S, C>> root, T k) {
    if (!root) return root;
    if (k < root->workExperience) {
        root->left = deleteNode(root->left, k);
    } else if (k > root->workExperience) {
        root->right = deleteNode(root->right, k);
    } else {
        if (!root->left || !root->right) {
            shared_ptr<node<T, S, C>> temp = root->left ? root->left : root->right;
            root = temp ? temp : nullptr;
        } else {
            shared_ptr<node<T, S, C>> temp = root->left;
            while (temp->right) temp = temp->right;
            root->workExperience = temp->workExperience;
            root->left = deleteNode(root->left, temp->workExperience);
        }
    }
    if (!root) return root;
    updateHeight(root);
    if (isAVL) {
        int balance = getBalance(root);
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }
    return root;
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::getRoot() {
    return root;
}

template <class T, class S, class C>
int AVL<T, S, C>::height(shared_ptr<node<T, S, C>> p) {
    return p ? p->height : 0;
}
