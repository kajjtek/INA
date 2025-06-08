#ifndef NODE_H
#define NODE_H

class Node {
    public:
    int key;
    int degree;
    Node* parent;
    Node* child;
    Node* sibling;

    Node(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

#endif