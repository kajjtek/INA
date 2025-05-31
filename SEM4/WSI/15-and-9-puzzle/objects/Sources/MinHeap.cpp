#include "../MinHeap.h" // Correct path to your header
#include <iostream>
#include <algorithm> // For std::swap
#include <climits>   // For INT_MAX and INT_MIN

// Constructor: No initialization needed for std::vector, it's default constructed
MinHeap::MinHeap()
{}

// Inserts a new key 'k'
void MinHeap::insertKey(int k)
{
    // No explicit overflow check needed, std::vector handles dynamic resizing.
    queue.push_back(k); // First insert the new key at the end
    int i = queue.size() - 1;

    // Fix the min heap property if it is violated
    // Use .at() or [] for access, both are fine here.
    while (i != 0 && queue.at(parent(i)) > queue.at(i))
    {
        std::swap(queue[i], queue[parent(i)]);
        i = parent(i);
    }
}

// Decreases value of key at index 'i' to new_val. It is assumed that
// new_val is smaller than harr[i].
void MinHeap::decreaseKey(int i, int new_val)
{
    // Basic bounds check, though the caller should ideally ensure i is valid.
    if (i < 0 || i >= queue.size()) {
        // Handle error, e.g., throw an exception, print a message, or return.
        std::cerr << "Error: Index out of bounds in decreaseKey." << std::endl;
        return;
    }
    // Also, usually you'd check new_val < queue[i] as a precondition.
    if (new_val > queue[i]) {
        std::cerr << "Warning: new_val is not smaller than current value in decreaseKey." << std::endl;
        // Depending on requirements, you might do nothing or throw.
    }


    queue[i] = new_val;
    while (i != 0 && queue.at(parent(i)) > queue.at(i))
    {
        std::swap(queue[i], queue[parent(i)]);
        i = parent(i);
    }
}

// Method to remove minimum element (or root) from min heap
int MinHeap::extractMin()
{
    if (queue.empty()) // Use .empty() for clarity
        return INT_MAX; // Or throw an exception for clarity about empty heap

    if (queue.size() == 1)
    {
        int value = queue.front(); // or queue[0]
        queue.pop_back(); // Removes the last element (which is the only element)
        return value;
    }

    // Store the minimum value, and remove it from heap
    int root = queue[0];
    queue[0] = queue.back(); // Move last element to root
    queue.pop_back();        // Remove the last element (which is now a duplicate at root)

    // Only heapify if there are elements left
    if (!queue.empty()) { // Important check
        MinHeapify(0);
    }


    return root;
}


// This function deletes key at index i. It first reduces value to minus
// infinite, then calls extractMin()
void MinHeap::deleteKey(int i)
{
    // Basic bounds check
    if (i < 0 || i >= queue.size()) {
        std::cerr << "Error: Index out of bounds in deleteKey." << std::endl;
        return;
    }

    // This logic relies on INT_MIN being smaller than any other possible value.
    // If you're storing custom objects, you'd need a custom 'minimum' value
    // or a different strategy for deletion.
    decreaseKey(i, INT_MIN);
    extractMin();
}

// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;

    // Corrected the swap condition in MinHeapify
    // It should swap with 'smallest' if 'smallest' is not 'i'
    // Your previous code was swapping queue[i] with queue[parent(i)] inside MinHeapify,
    // which is incorrect for heapify logic. It needs to swap with the 'smallest' child.
    if (l < queue.size() && queue[l] < queue[smallest]) // Check against smallest, not i
        smallest = l;
    if (r < queue.size() && queue[r] < queue[smallest])
        smallest = r;

    if (smallest != i)
    {
        std::swap(queue[i], queue[smallest]); // Corrected swap target
        MinHeapify(smallest);
    }
}

bool MinHeap::contains(int k) const
{
    // Iterate through the entire vector to find the key
    for (int val : queue) { // Range-based for loop for convenience
        if (val == k) {
            return true; // Found the key
        }
    }
    return false; // Key not found
}