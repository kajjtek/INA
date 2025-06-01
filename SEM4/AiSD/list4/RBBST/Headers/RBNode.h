#ifndef RBNODE_H
#define RBNODE_H

#include "./Color.h" 

class RBNode {
    public:
    int value;
    Color color;
    RBNode* parent = nullptr;
    RBNode* right = nullptr;
    RBNode* left = nullptr;

    /* CONSTRUCTOR */
    RBNode(int value, RBNode* parent, Color color, RBNode* nil){
        this->color=color;
        this->value=value;
        this->parent=parent;
        this->left=nil;
        this->right=nil;
    }

    RBNode(){
        this->color=Color::Black;
        this->value=0;
        this->parent=this;
        this->left=this;
        this->right=this;
    }

    ~RBNode(){}
};

#endif