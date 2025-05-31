#ifndef TREE_H
#define TREE_H

#include "./Node.h"

class Tree {
    public:
    Node* root;
    Tree(int root){
        this->root=new Node(root, nullptr);
    }
    ~Tree(){delete root;}
};

#endif