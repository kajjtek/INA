#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include "Node.h"
#include <vector>

class BinomialHeap {
private:
    Node* head;
    int comparisons;

    void linkTrees(Node* y, Node* z);
    Node* mergeHeaps(Node* h1, Node* h2);
    Node* unionHeaps(Node* h1, Node* h2);
    void printTree(Node* node, int indent = 0);

public:
    BinomialHeap();

    void insert(int key);
    void merge(BinomialHeap& other);
    int extractMin();
    bool isEmpty() const;

    int getComparisons() const;
    void resetComparisons();
};

#endif
