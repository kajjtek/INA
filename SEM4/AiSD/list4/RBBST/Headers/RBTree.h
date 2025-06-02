#ifndef RBTREE_H
#define RBTREE_H

#include "./RBNode.h"
#include <vector>
#include <iostream>

class RBTree {
    public:
    static RBNode* NIL;
    RBNode* root;

    static long long key_comparisons;
    static long long pointer_operations;

    RBTree(){
        if (NIL == nullptr) {
            NIL = new RBNode();
            NIL->parent = NIL; 
            NIL->left = NIL;
            NIL->right = NIL;
            NIL->color = Color::Black;
        }
        this->root=NIL;
    }

    static void reset_counts(){
        key_comparisons = 0;
        pointer_operations = 0;
    }

    int height();
    void insert(int key);
    RBNode* findSuccessor(RBNode* current);
    void deletion(int key);
    std::vector<RBNode*> inOrderTraversal();
    void doInOrderTraversal(RBNode* current, std::vector<RBNode*> &result);
    void print_BST();
    void clean();
    ~RBTree(){
        clean();
    }

    static void reset_counters() {
        key_comparisons = 0;
        pointer_operations = 0;
    }

    private:
    void doPrint(RBNode* current, int depth, char prefix, std::vector<bool>& leftTrace, std::vector<bool>& rightTrace);
    void doClean(RBNode* current);
    void deletionRoot();
    int calculateHeight(RBNode* current);
    void doInsert(RBNode* current, int key);
    void doDeletion(RBNode* current, int key);
    void recoloring(RBNode* current);
    void LLRotation(RBNode* x, RBNode* parent, RBNode* grandfather);
    void LRRotation(RBNode* x, RBNode* parent, RBNode* grandfather);
    void RRRotation(RBNode* x, RBNode* parent, RBNode* grandfather);
    void RLRotation(RBNode* x, RBNode* parent, RBNode* grandfather);
    bool isBlack(RBNode* x){
        if (x == NIL) {
             pointer_operations++;
             return true;
        }
        pointer_operations++;
        pointer_operations++;
        return x->color==Color::Black || x->color==Color::DoubleBlack;
    }
    bool isRed(RBNode* x){
        pointer_operations++;
        if(x==NIL) return false;
        pointer_operations++;
        return x->color==Color::Red;
    }
    void findRotation(RBNode* x, RBNode* parent, RBNode* grandfather);
    void fixDoubleBlack(RBNode* child, RBNode* parent);
};

#endif