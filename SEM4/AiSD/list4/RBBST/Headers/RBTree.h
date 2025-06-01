#ifndef RBTREE_H
#define RBTREE_H

#include "./RBNode.h"
#include <vector>
#include <iostream>

class RBTree {
    public:
    static RBNode* NIL;
    RBNode* root;
    RBTree(){
        this->NIL=NIL==nullptr?new RBNode():NIL;
        this->root=this->NIL;
    }
    int height();
    void insert(int key);
    RBNode* findSuccessor(RBNode* current);
    void deletion(int key);
    std::vector<RBNode*> inOrderTraversal();
    void doInOrderTraversal(RBNode* current, std::vector<RBNode*> &result);
    void print_BST();
    ~RBTree(){clean(); if(NIL!=nullptr){delete NIL; NIL=nullptr;}} //post-order traversal that delets all the elements
    private:
    void doPrint(RBNode* current, int depth, char prefix, std::vector<bool>& leftTrace, std::vector<bool>& rightTrace);
    void clean();
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
    bool isBlack(RBNode* x){return x->color==Color::Black || x==NIL || x->color==Color::DoubleBlack;}
    bool isRed(RBNode* x){
        if(x==NIL) return false;
        return x->color==Color::Red;}
    void findRotation(RBNode* x, RBNode* parent, RBNode* grandfather);
    void fixDoubleBlack(RBNode* child, RBNode* parent);
    // void fixDoubleBlackWithParent(RBNode* child, RBNode* parent);
};

#endif