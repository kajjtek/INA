#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include "Node.h"

class BinomialHeap {
private:
    Node* head;
    mutable long long currentOperationComparisons;
    mutable long long totalComparisonsMade;

    void deleteAllNodes(Node* node);

    void link(Node* y, Node* z);

    Node* merge(Node* H1Head, Node* H2Head);

public:
    BinomialHeap() : head(nullptr), currentOperationComparisons(0), totalComparisonsMade(0) {}

    ~BinomialHeap() {
        deleteAllNodes(head);
        head = nullptr;
    }

    void heapInsert(int key);

    Node* extractMin();

    void heapUnion(BinomialHeap& H2);

    long long getComparisonCount() const {
        return currentOperationComparisons;
    }

    void resetComparisonCount() {
        currentOperationComparisons = 0;
    }

    long long getTotalComparisonCount() const {
        return totalComparisonsMade;
    }

    Node* getHead() const {
        return head;
    }
};

#endif