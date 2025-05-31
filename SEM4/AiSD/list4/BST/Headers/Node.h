#ifndef NODE_H
#define NODE_H



class Node {
    public:
    int value;
    Node* parent = nullptr;
    Node* right = nullptr;
    Node* left = nullptr;

    /* CONSTRUCTOR */
    Node(int value, Node* parent){
        this->value=value;
        this->parent=parent;
    }

    /* FUNCTIONS */
    void insert(int key){
        if(key>=value){
            if(right==nullptr) this->right=new Node(key, this);
            else right->insert(key);
        }else{
            if(left==nullptr) this->left=new Node(key, this);
            else left->insert(key);
        }
    }

    void deletion(int key){
        if(key==this->value){
            // case 1 no children
            if(this->left==nullptr && this->right==nullptr){
                parent->deleteChild(this);
            }
            // case 3 two children
            else if(this->left!=nullptr && this->right!=nullptr){
                Node* successor = findSuccessor();
                this->value=successor->value;
                successor->deletion(successor->value);
            }
            // case 2 one children
            else {
                Node* child = right!=nullptr?right:left;
                child->parent=this->parent;
                if(this->parent->right==this) this->parent->right=child;
                else this->parent->left=child;
                this->parent->deleteChild(this);
            }
        }
    }

    void deleteChild(Node* child){
        if(this->left==child) this->left=nullptr;
        else if(this->right==child) this->right=nullptr;
        delete child;
    }

    Node* findSuccessor(){
        Node* current = this;
        while(current!=nullptr){
            current=current->left;
        }
        return current;
    }

    ~Node(){}
};

#endif