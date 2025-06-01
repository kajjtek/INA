#ifndef STREE_H
#define STREE_H

#include "./SNode.h"
#include <vector>
#include <iostream>

class STree {
    public:
    static SNode* NIL;
    SNode* root;
    STree(){
        this->NIL=NIL==nullptr?new SNode():NIL;
        this->root=this->NIL;
    }
    int height();
    void insert(int key);
    SNode* findSuccessor(SNode* current);
    void deletion(int key);
    std::vector<SNode*> inOrderTraversal();
    void doInOrderTraversal(SNode* current, std::vector<SNode*> &result);
    void print_BST();
    ~STree(){clean(); if(NIL!=nullptr){delete NIL; NIL=nullptr;}} //post-order traversal that delets all the elements
    private:
    void doInsert(SNode* current, int key);
    void doPrint(SNode* current, int depth, char prefix, std::vector<bool>& leftTrace, std::vector<bool>& rightTrace);
    void clean();
    void doClean(SNode* current);
    void deletionRoot();
    int calculateHeight(SNode* current);
    void doDeletion(SNode* current, int key);
    void Zig(SNode* x);
    void Zag(SNode* x);
    void ZigZig(SNode* x);
    void ZagZag(SNode* x);
    void ZigZag(SNode* x);
    void ZagZig(SNode* x);
    void Splay(SNode* x);
    void SplayFind(int key);
    SNode* find(int key);
    SNode* findMinimum(SNode* x);
};

#endif