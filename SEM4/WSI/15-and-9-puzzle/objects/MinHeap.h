#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include <climits> // For INT_MAX in extractMin and INT_MIN in deleteKey
                   // Although extractMin's usage here might be problematic if INT_MAX is a valid puzzle state.
                   // More on this below.

class MinHeap
{
    std::vector<int> queue; // pointer to array of elements in heap (comment is a bit outdated, it's not a pointer now)

  public:
    // Constructor
    MinHeap();

    // to heapify a subtree with the root at given index
    void MinHeapify(int);

    int parent(int i) { return (i - 1) / 2; }

    // to get index of left child of node at index i
    int left(int i) { return (2 * i + 1); }

    // to get index of right child of node at index i
    int right(int i) { return (2 * i + 2); }

    // to extract the root which is the minimum element
    int extractMin();

    // Decreases key value of key at index i to new_val
    void decreaseKey(int i, int new_val);

    // Returns the minimum key (key at root) from min heap
    // Added const correctness. Also, .at() can throw, [] is often preferred for performance if bounds are checked.
    int getMin() const { return queue.empty() ? INT_MAX : queue[0]; } // Handle empty heap case for getMin

    // Deletes a key stored at index i
    void deleteKey(int i);

    // Inserts a new key 'k'
    void insertKey(int k);

    // Add a helper for checking if the heap is empty, useful for the 15-puzzle
    bool isEmpty() const { return queue.empty(); }

    // Add size accessor
    size_t size() const { return queue.size(); }

    bool contains(int k) const; // Added this line
};

#endif