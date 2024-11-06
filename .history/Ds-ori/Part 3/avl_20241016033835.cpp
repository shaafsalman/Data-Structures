#include <iostream>
#include <memory>
#include <algorithm>
#include "avl.h"

template <class T, class S, class C>
AVL<T, S, C>::AVL(bool isAVL) 
{
    this->isAVL = isAVL;
    this->root = nullptr;
}

template <class T, class S, class C>
void AVL<T, S, C>::insertNode(shared_ptr<node<T, S, C>> newNode) 
{
    if (isAVL) 
    {
        root = insertAVLHelper(root, newNode);
    } 
    else 
    {
        root = insertBSTHelper(root, newNode);
    }
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertAVLHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode) 
{
    if (current == nullptr) 
    {
        return newNode;
    }

    if (newNode->workExperience < current->workExperience) 
    {
        current->left = insertAVLHelper(current->left, newNode);
    } 
    else if (newNode->workExperience > current->workExperience) 
    {
        current->right = insertAVLHelper(current->right, newNode);
    } 
    else 
    {
        return current;
    }

    return balanceTree(current);
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertBSTHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode) 
{
    if (current == nullptr) 
    {
        return newNode;
    }

    if (newNode->workExperience < current->workExperience) 
    {
        current->left = insertBSTHelper(current->left, newNode);
    } 
    else if (newNode->workExperience > current->workExperience) 
    {
        current->right = insertBSTHelper(current->right, newNode);
    }

    return current;
}

template <class T, class S, class C>
int AVL<T, S, C>::height(shared_ptr<node<T, S, C>> p)
{
    if (p == nullptr) 
    {
        return 0;
    } 
    else 
    {
        return p->height;
    }
}

template <class T, class S, class C>
int AVL<T, S, C>::getBalanceFactor(shared_ptr<node<T, S, C>> p) 
{
    if (p == nullptr) 
    {
        return 0;
    } 
    else 
    {
        return height(p->left) - height(p->right);
    }
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateRight(shared_ptr<node<T, S, C>> y) 
{
    shared_ptr<node<T, S, C>> x = y->left;
    shared_ptr<node<T, S, C>> T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateLeft(shared_ptr<node<T, S, C>> x) 
{
    shared_ptr<node<T, S, C>> y = x->right;
    shared_ptr<node<T, S, C>> T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::balanceTree(shared_ptr<node<T, S, C>> current) 
{
    if (current == nullptr) 
    {
        return current;
    }

    current->height = std::max(height(current->left), height(current->right)) + 1;
    int balanceFactor = getBalanceFactor(current);

    if (balanceFactor > 1) 
    {
        if (getBalanceFactor(current->left) < 0) 
        {
            current->left = rotateLeft(current->left);
        }
        return rotateRight(current);
    }

    if (balanceFactor < -1) 
    {
        if (getBalanceFactor(current->right) > 0) 
        {
            current->right = rotateRight(current->right);
        }
        return rotateLeft(current);
    }

    return current;
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchNode(T k) 
{
    return searchHelper(root, k);
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::searchHelper(shared_ptr<node<T, S, C>> current, T k) 
{
    if (current == nullptr || current->workExperience == k) 
    {
        return current;
    }

    if (k < current->workExperience) 
    {
        return searchHelper(current->left, k);
    }

    return searchHelper(current->right, k);
}

template <class T, class S, class C>
void AVL<T, S, C>::deleteNode(T k) 
{
    if (isAVL) 
    {
        root = deleteAVLHelper(root, k);
    } 
    else 
    {
        root = deleteBSTHelper(root, k);
    }
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::deleteAVLHelper(shared_ptr<node<T, S, C>> current, T k) 
{
    if (current == nullptr) 
    {
        return current;
    }

    if (k < current->workExperience) 
    {
        current->left = deleteAVLHelper(current->left, k);
    } 
    else if (k > current->workExperience) 
    {
        current->right = deleteAVLHelper(current->right, k);
    } 
    else 
    {
        if (current->left == nullptr || current->right == nullptr) 
        {
            return current->left ? current->left : current->right;
        } 
        else 
        {
            shared_ptr<node<T, S, C>> temp = current->left;
            while (temp->right != nullptr) 
            {
                temp = temp->right;
            }
            current->workExperience = temp->workExperience;
            current->fullName = temp->fullName;
            current->gender = temp->gender;
            current->left = deleteAVLHelper(current->left, temp->workExperience);
        }
    }

    return balanceTree(current);
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::deleteBSTHelper(shared_ptr<node<T, S, C>> current, T k) 
{
    if (current == nullptr) 
    {
        return current;
    }

    if (k < current->workExperience) 
    {
        current->left = deleteBSTHelper(current->left, k);
    } 
    else if (k > current->workExperience) 
    {
        current->right = deleteBSTHelper(current->right, k);
    } 
    else 
    {
        if (current->left == nullptr || current->right == nullptr) 
        {
            return current->left ? current->left : current->right;
        } 
        else 
        {
            shared_ptr<node<T, S, C>> temp = current->left;
            while (temp->right != nullptr) 
            {
                temp = temp->right;
            }
            current->workExperience = temp->workExperience;
            current->fullName = temp->fullName;
            current->gender = temp->gender;
            current->left = deleteBSTHelper(current->left, temp->workExperience);
        }
    }

    return current;
}

template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::getRoot() 
{
    return root;
}
