#include "../Header/BinomialHeap.h"
#include <algorithm>

void BinomialHeap::deleteAllNodes(Node* node) {
        if (node == nullptr) return;
        Node* current = node;
        while (current != nullptr) {
            Node* nextSibling = current->sibling;
            deleteAllNodes(current->child);
            delete current;
            current = nextSibling;
        }
    }

void BinomialHeap::link(Node* y, Node* z) {
    currentOperationComparisons++;
    if (y->key > z->key) {
        std::swap(y, z);
    }
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

Node* BinomialHeap::merge(Node* H1Head, Node* H2Head) {
    if (H1Head == nullptr) return H2Head;
    if (H2Head == nullptr) return H1Head;

    Node* newHead = nullptr;
    Node* tail = nullptr;

    currentOperationComparisons++;
    if (H1Head->degree <= H2Head->degree) {
        newHead = H1Head;
        H1Head = H1Head->sibling;
    } else {
        newHead = H2Head;
        H2Head = H2Head->sibling;
    }
    tail = newHead;

    while (H1Head != nullptr && H2Head != nullptr) {
        currentOperationComparisons++;
        if (H1Head->degree <= H2Head->degree) {
            tail->sibling = H1Head;
            H1Head = H1Head->sibling;
        } else {
            tail->sibling = H2Head;
            H2Head = H2Head->sibling;
        }
        tail = tail->sibling;
    }

    if (H1Head != nullptr) {
        tail->sibling = H1Head;
    } else if (H2Head != nullptr) {
        tail->sibling = H2Head;
    }
    return newHead;
}


void BinomialHeap::heapInsert(int key) {
    currentOperationComparisons = 0;
    Node* x = new Node(key);
    BinomialHeap tempHeap;
    tempHeap.head = x;
    heapUnion(tempHeap);
    tempHeap.head = nullptr;
    totalComparisonsMade += currentOperationComparisons;
}

Node* BinomialHeap::extractMin() {
    currentOperationComparisons = 0;
    if (head == nullptr) return nullptr;

    Node* minNode = head;
    Node* minPrev = nullptr;
    Node* current = head->sibling;
    Node* prev = head;

    while (current != nullptr) {
        currentOperationComparisons++;
        if (current->key < minNode->key) {
            minNode = current;
            minPrev = prev;
        }
        prev = current;
        current = current->sibling;
    }

    if (minNode == head) {
        head = minNode->sibling;
    } else {
        minPrev->sibling = minNode->sibling;
    }
    minNode->sibling = nullptr;

    BinomialHeap tempHeapFromChildren;
    Node* childHead = nullptr;
    Node* currentChild = minNode->child;
    Node* nextChild;

    while (currentChild != nullptr) {
        currentChild->parent = nullptr;
        nextChild = currentChild->sibling;
        currentChild->sibling = childHead;
        childHead = currentChild;
        currentChild = nextChild;
    }
    tempHeapFromChildren.head = childHead;

    heapUnion(tempHeapFromChildren);
    tempHeapFromChildren.head = nullptr;

    totalComparisonsMade += currentOperationComparisons;
    return minNode;
}

void BinomialHeap::heapUnion(BinomialHeap& H2) {
    currentOperationComparisons = 0;

    Node* H1HeadOriginal = this->head;
    Node* H2HeadOriginal = H2.head;

    Node* mergedHead = merge(H1HeadOriginal, H2HeadOriginal);

    this->head = nullptr;
    H2.head = nullptr;

    if (mergedHead == nullptr) {
        totalComparisonsMade += currentOperationComparisons;
        return;
    }

    Node* prevX = nullptr;
    Node* x = mergedHead;
    Node* nextX = x->sibling;

    while (nextX != nullptr) {
        bool degreesDiffer = (x->degree != nextX->degree);
        currentOperationComparisons++;

        bool nextNextHasSameDegree = false;
        if (nextX->sibling != nullptr) {
            currentOperationComparisons++;
            nextNextHasSameDegree = (nextX->sibling->degree == x->degree);
        }

        if (degreesDiffer || nextNextHasSameDegree) {
            prevX = x;
            x = nextX;
        } else {
            if (x->key <= nextX->key) {
                x->sibling = nextX->sibling;
                link(nextX, x);
            } else {
                if (prevX == nullptr) {
                    mergedHead = nextX;
                } else {
                    prevX->sibling = nextX;
                }
                link(x, nextX);
                x = nextX;
            }
        }
        nextX = x->sibling;
    }
    this->head = mergedHead;
    totalComparisonsMade += currentOperationComparisons;
}