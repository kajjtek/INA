#ifndef SNODE_H
#define SNODE_H

class SNode {
    public:
    int value;
    SNode* parent = nullptr;
    SNode* right = nullptr;
    SNode* left = nullptr;

    /* CONSTRUCTOR */
    SNode(int value, SNode* parent,  SNode* nil){
        this->value=value;
        this->parent=parent;
        this->left=nil;
        this->right=nil;
    }

    SNode(){
        this->value=0;
        this->parent=this;
        this->left=this;
        this->right=this;
    }

    ~SNode(){}
};

#endif