#ifndef TREE_H
#define TREE_H

#include "./Node.h"
#include <vector>
#include <iostream>

class Tree {
    public:
    static Node* NIL;
    Node* root;

    static long long comparisons;
    static long long pointer_operations;

    Tree(){
        this->NIL=NIL==nullptr?new Node():NIL;
        this->root=this->NIL;
    }
    static void reset_counters(){
        comparisons = 0;
        pointer_operations = 0;
    }
    int height();
    void insert(int key);
    Node* findSuccessor(Node* current);
    void deletion(int key);
    std::vector<Node*> inOrderTraversal();
    void doInOrderTraversal(Node* current, std::vector<Node*> &result);
    void print_BST();
    ~Tree(){clean();} //post-order traversal that delets all the elements
    private:
    void doInsert(Node* current, int key);
    void doPrint(Node* current, int depth, char prefix, std::vector<bool>& leftTrace, std::vector<bool>& rightTrace);
    void clean();
    void doClean(Node* current);
    void deletionRoot();
    int calculateHeight(Node* current);
    void doDeletion(Node* current, int key);
};

#endif