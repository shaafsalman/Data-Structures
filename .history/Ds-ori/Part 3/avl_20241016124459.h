#include <memory>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;

#ifndef __AVL_H
#define __AVL_H


template <class T, class S, class C>
struct node
{
    S fullName;                
    T workExperience;       
    C gender;               
    shared_ptr<node> left;   
    shared_ptr<node> right;    
    int height;                

    node(T w, S n, C g)
    {
        this->fullName = n;
        this->workExperience = w;
        this->gender = g;
        left = nullptr;
        right = nullptr;
        height = 1;           
    }
};


template <class T, class S, class C>
class AVL
{
    shared_ptr<node<T, S, C>> root; 
    bool isAVL;                     

public:
    AVL(bool isAVL);
  
    void insertNode(shared_ptr<node<T, S, C>> newNode);  
    void deleteNode(T k);                           
    shared_ptr<node<T, S, C>> getRoot();                
    shared_ptr<node<T, S, C>> searchNode(T k);          

    //helpers
    shared_ptr<node<T, S, C>> insertHelper(shared_ptr<node<T, S, C>> current, shared_ptr<node<T, S, C>> newNode); 
    shared_ptr<node<T, S, C>> deleteHelper(shared_ptr<node<T, S, C>> current, T k);
    int height(shared_ptr<node<T, S, C>> p);
    shared_ptr<node<T, S, C>> searchHelper(shared_ptr<node<T, S, C>> current, T k);


    // Balance functions 
    int getBalanceFactor(shared_ptr<node<T, S, C>> p);               
    shared_ptr<node<T, S, C>> rotateRight(shared_ptr<node<T, S, C>> y);
    shared_ptr<node<T, S, C>> rotateLeft(shared_ptr<node<T, S, C>> x);  
    shared_ptr<node<T, S, C>> balanceTree(shared_ptr<node<T, S, C>> current); 

     // traversals
    void inOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result);      
    void levelOrderTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result);   
    void rightMostTraversal(shared_ptr<node<T, S, C>> root, vector<T>& result);  


    // Part 4 Functions 
    int number_to_shortlist(shared_ptr<node<T, S, C>> root);
    vector<T> right_most(shared_ptr<node<T, S, C>> root);   
    vector<T> in_order(shared_ptr<node<T, S, C>> root);     
    vector<T> level_order(shared_ptr<node<T, S, C>> root);  
    vector<float> bias(shared_ptr<node<T, S, C>> root);      
    void biasCalculation(shared_ptr<node<T, S, C>> root, vector<float>& result);

};

#endif