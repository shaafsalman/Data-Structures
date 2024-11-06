#include <iostream>
#include <queue>
#include <stack>
#include <memory>
#include "../Part 3/avl.h"

template <class T, class S, class C>
int AVL<T, S, C>::number_to_shortlist(shared_ptr<node<T, S, C>> root)
{
    if (!root) 
        return 0;
    
    int height = 0;
    while (root) 
    {
        height++;
        root = root->right;
    }
    return height;
}

template <class T, class S, class C>
vector<T> AVL<T, S, C>::right_most(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    while (root) 
    {
        result.push_back(root->workExperience);
        root = root->right;
    }
    return result;
}

template <class T, class S, class C>
vector<T> AVL<T, S, C>::in_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    int limit = number_to_shortlist(root);
    inOrderTraversal(root, result);
    
    if (result.size() > limit) 
    {
        result.resize(limit);
    }
    return result;
}

template <class T, class S, class C>
vector<T> AVL<T, S, C>::level_order(shared_ptr<node<T, S, C>> root)
{
    vector<T> result;
    if (!root) 
        return result;
    
    int limit = number_to_shortlist(root);
    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);
    
    while (!q.empty() && result.size() < limit) 
    {
        auto current = q.front();
        q.pop();
        result.push_back(current->workExperience);
        if (current->left) 
            q.push(current->left);
        if (current->right) 
            q.push(current->right);
    }
    return result;
}

template <class T, class S, class C>
void AVL<T, S, C>::inOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result)
{
    if (!root) 
        return;
    
    inOrderTraversal(root->left, result);
    result.push_back(root->workExperience);
    inOrderTraversal(root->right, result);
}

template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root)
{
    vector<float> result;
    if (!root) 
        return result;
    
    auto countGenders = [](shared_ptr<node<T, S, C>> subtree) -> pair<int, int> 
    {
        int males = 0, females = 0;
        if (subtree) 
        {
            stack<shared_ptr<node<T, S, C>>> s;
            s.push(subtree);
            while (!s.empty()) 
            {
                auto current = s.top();
                s.pop();
                if (current->gender == "M") 
                    males++;
                else if (current->gender == "F") 
                    females++;
                if (current->left) 
                    s.push(current->left);
                if (current->right) 
                    s.push(current->right);
            }
        }
        return {males, females};
    };

    queue<shared_ptr<node<T, S, C>>> q;
    q.push(root);
    
    while (!q.empty()) 
    {
        auto current = q.front();
        q.pop();
        
        if (current->left) 
        {
            auto [males, females] = countGenders(current->left);
            float ratio = (males + females == 0) ? 0 : (males == 0 ? 1 : static_cast<float>(females) / (males + females));
            result.push_back(ratio);
        }
        
        if (current->left) 
            q.push(current->left);
        if (current->right) 
            q.push(current->right);
    }
    
    return result;
}
