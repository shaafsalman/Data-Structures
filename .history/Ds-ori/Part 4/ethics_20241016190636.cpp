#include <iostream>
#include <queue>
#include <stack>

#include <memory>
#include "../Part 3/avl.h"

//------------------------
// Number to Shortlist
//------------------------
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

//------------------------
// Right-Most Path
//------------------------
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

//------------------------
// In-Order Traversal
//------------------------
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

//------------------------
// Level-Order Traversal
//------------------------
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

//------------------------
// In-Order Traversal Helper
//------------------------
template <class T, class S, class C>
void AVL<T, S, C>::inOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result)
{
    if (!root)
        return;

    inOrderTraversal(root->left, result);
    result.push_back(root->workExperience);
    inOrderTraversal(root->right, result);
}

//------------------------
// Gender Bias Calculation
//------------------------

// The bias ratios represent the proportion of female applicants to total applicants
// (male and female) in each left subtree of the tree. The ratio approaches 1 when
// there are significantly more females than males and 0 when there are none. A ratio
// greater than 0 but less than 1 shows a mix of both genders. 

// ratios alone dont directly prove bias in terms of avg work experience,
// but it gives insight into gender representation at different parts of the tree.
// A detailed analysis combining work experience and gender proportions would be
// necessary to find about bias due to work experience.

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
