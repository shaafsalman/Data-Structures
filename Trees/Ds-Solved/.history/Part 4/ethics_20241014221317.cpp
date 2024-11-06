#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <functional>
using namespace std;

// Node of the tree
template <class T, class S, class C>
struct node {
    S fullName;
    T workExperience;
    string gender;
    shared_ptr<node> left;
    shared_ptr<node> right;
    int height;

    node(T w, S n, C g) {
        this->fullName = n;
        this->workExperience = w;
        this->gender = g;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

// AVL Class
template <class T, class S, class C>
class AVL {
    shared_ptr<node<T, S, C>> root;

public:
    AVL() : root(nullptr) {}
    void insertNode(shared_ptr<node<T, S, C>> newNode);
    void deleteNode(T k);
    shared_ptr<node<T, S, C>> getRoot() { return root; }
    shared_ptr<node<T, S, C>> searchNode(T k);
    int height(shared_ptr<node<T, S, C>> p);

    int number_to_shortlist(shared_ptr<node<T, S, C>> root);
    vector<T> right_most(shared_ptr<node<T, S, C>> root);
    vector<T> in_order(shared_ptr<node<T, S, C>> root);
    vector<T> level_order(shared_ptr<node<T, S, C>> root);
    vector<float> bias(shared_ptr<node<T, S, C>> root);

private:
    shared_ptr<node<T, S, C>> insertHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode);
    shared_ptr<node<T, S, C>> deleteHelper(shared_ptr<node<T, S, C>> current, T k);
    shared_ptr<node<T, S, C>> searchHelper(shared_ptr<node<T, S, C>> current, T k);
    shared_ptr<node<T, S, C>> balanceTree(shared_ptr<node<T, S, C>> current);
    shared_ptr<node<T, S, C>> rotateRight(shared_ptr<node<T, S, C>> y);
    shared_ptr<node<T, S, C>> rotateLeft(shared_ptr<node<T, S, C>> x);
    shared_ptr<node<T, S, C>> findMax(shared_ptr<node<T, S, C>> current);
    int getBalanceFactor(shared_ptr<node<T, S, C>> current);
};

// Function to insert a new node
template <class T, class S, class C>
void AVL<T, S, C>::insertNode(shared_ptr<node<T, S, C>> newNode) {
    root = insertHelper(root, newNode);
}

// Insert helper function
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::insertHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode) {
    if (!current) return newNode;

    if (newNode->workExperience < current->workExperience) {
        current->left = insertHelper(current->left, newNode);
    } else {
        current->right = insertHelper(current->right, newNode);
    }

    // Balance the tree
    return balanceTree(current);
}

// Function to balance the tree
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::balanceTree(shared_ptr<node<T, S, C>> current) {
    current->height = 1 + max(height(current->left), height(current->right));
    int balance = getBalanceFactor(current);

    // Left heavy
    if (balance > 1) {
        if (getBalanceFactor(current->left) < 0) {
            current->left = rotateLeft(current->left);
        }
        return rotateRight(current);
    }
    // Right heavy
    if (balance < -1) {
        if (getBalanceFactor(current->right) > 0) {
            current->right = rotateRight(current->right);
        }
        return rotateLeft(current);
    }
    return current;
}

// Function to rotate right
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateRight(shared_ptr<node<T, S, C>> y) {
    auto x = y->left;
    auto T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x; // New root
}

// Function to rotate left
template <class T, class S, class C>
shared_ptr<node<T, S, C>> AVL<T, S, C>::rotateLeft(shared_ptr<node<T, S, C>> x) {
    auto y = x->right;
    auto T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y; // New root
}

// Function to get balance factor
template <class T, class S, class C>
int AVL<T, S, C>::getBalanceFactor(shared_ptr<node<T, S, C>> current) {
    if (!current) return 0;
    return height(current->left) - height(current->right);
}

// Function to calculate the height of a node
template <class T, class S, class C>
int AVL<T, S, C>::height(shared_ptr<node<T, S, C>> p) {
    return p ? p->height : 0;
}

// Function to return total number of applicants that can be shortlisted
template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root) {
    if (!root) return 0;
    return height(root); // Height gives the total number of shortlisted candidates
}

// Function to return shortlisted candidates on the rightmost path to leaf
template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    auto current = root;
    while (current) {
        result.push_back(current->workExperience);
        current = current->right; // Move to the right child
    }
    return result;
}

// Function to return shortlisted candidates in-order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    function<void(shared_ptr<node<T, S, C>>)> inOrderHelper = [&](shared_ptr<node<T, S, C>> node) {
        if (node) {
            inOrderHelper(node->left);
            result.push_back(node->workExperience);
            inOrderHelper(node->right);
        }
    };
    inOrderHelper(root);
    return result;
}

// Function to return shortlisted candidates in level order
template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root) {
    vector<T> result;
    if (!root) return result;

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        result.push_back(current->workExperience);

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    return result;
}

// Function to calculate the bias in the tree
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    vector<float> ratios;
    if (!root) return ratios;

    // For each node in level order, calculate the ratio of females to males in the left subtree
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        // Count males and females in the left subtree
        int maleCount = 0, femaleCount = 0;
        function<void(shared_ptr<node<T, S, C>>)> countHelper = [&](shared_ptr<node<T, S, C>> node) {
            if (node) {
                if (node->gender == "Male") maleCount++;
                else if (node->gender == "Female") femaleCount++;
                countHelper(node->left);
                countHelper(node->right);
            }
        };
        countHelper(current->left); // Count in left subtree

        // Calculate ratio and push to ratios vector
        float ratio = femaleCount + maleCount > 0 ? static_cast<float>(femaleCount) / (femaleCount + maleCount) : 0.0;
        ratios.push_back(ratio);

        // Add children to the queue
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return ratios;
}

