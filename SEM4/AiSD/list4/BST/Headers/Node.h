#ifndef NODE_H
#define NODE_H



class Node {
    public:
    int value;
    Node* parent = nullptr;
    Node* right = nullptr;
    Node* left = nullptr;

    /* CONSTRUCTOR */
    Node(int value, Node* parent, Node* nil){
        this->value=value;
        this->parent=parent;
        this->left=nil;
        this->right=nil;
    }

    Node(){
        this->value=0;
        this->parent=this;
        this->left=this;
        this->right=this;
    }

};

#endif