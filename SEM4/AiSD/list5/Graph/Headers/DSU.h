#ifndef DSU_H
#define DSU_H

#include <map>
#include "Node.h"

class DSU {
    public:
    std::unordered_map<Node*, Node*> parentMap;
    std::unordered_map<Node*, int> rankMap;
    void make_set(Node* node);
    Node* find_set(Node* node);
    void union_sets(Node* n1, Node* n2);
};

#endif