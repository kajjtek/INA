#include "../Header/BinomialHeap.h"
#include <iostream>
#include <climits>
#include <queue>

BinomialHeap::BinomialHeap() : head(nullptr), comparisons(0) {}

void BinomialHeap::linkTrees(Node* y, Node* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

Node* BinomialHeap::mergeHeaps(Node* h1, Node* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    Node* head = nullptr;
    Node* tail = nullptr;

    Node* a = h1;
    Node* b = h2;

    if (a->degree <= b->degree) {
        head = a;
        a = a->sibling;
    } else {
        head = b;
        b = b->sibling;
    }

    tail = head;

    while (a && b) {
        if (a->degree <= b->degree) {
            tail->sibling = a;
            a = a->sibling;
        } else {
            tail->sibling = b;
            b = b->sibling;
        }
        tail = tail->sibling;
    }

    tail->sibling = (a) ? a : b;

    return head;
}

Node* BinomialHeap::unionHeaps(Node* h1, Node* h2) {
    Node* newHead = mergeHeaps(h1, h2);
    if (!newHead) return nullptr;

    Node* prev = nullptr;
    Node* curr = newHead;
    Node* next = curr->sibling;

    while (next) {
        if ((curr->degree != next->degree) ||
            (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            comparisons++;
            if (curr->key <= next->key) {
                curr->sibling = next->sibling;
                linkTrees(next, curr);
            } else {
                if (!prev) {
                    newHead = next;
                } else {
                    prev->sibling = next;
                }
                linkTrees(curr, next);
                curr = next;
            }
        }
        next = curr->sibling;
    }

    return newHead;
}

void BinomialHeap::insert(int key) {
    BinomialHeap temp;
    temp.head = new Node(key);
    this->head = unionHeaps(this->head, temp.head);
}

void BinomialHeap::merge(BinomialHeap& other) {
    this->head = unionHeaps(this->head, other.head);
    other.head = nullptr;
}

int BinomialHeap::extractMin() {
    if (!head) return INT_MIN;

    Node* minNode = head;
    Node* minPrev = nullptr;
    Node* curr = head;
    Node* prev = nullptr;

    int minKey = minNode->key;

    while (curr) {
        comparisons++;
        if (curr->key < minKey) {
            minKey = curr->key;
            minNode = curr;
            minPrev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    if (minPrev) {
        minPrev->sibling = minNode->sibling;
    } else {
        head = minNode->sibling;
    }

    Node* child = minNode->child;
    Node* reversed = nullptr;

    while (child) {
        Node* next = child->sibling;
        child->sibling = reversed;
        child->parent = nullptr;
        reversed = child;
        child = next;
    }

    head = unionHeaps(head, reversed);
    delete minNode;
    return minKey;
}

bool BinomialHeap::isEmpty() const {
    return head == nullptr;
}

int BinomialHeap::getComparisons() const {
    return comparisons;
}

void BinomialHeap::resetComparisons() {
    comparisons = 0;
}
