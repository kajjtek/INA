#include "../Headers/Tree.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

void runTest(const std::string& testName, std::vector<int> insertKeys, std::vector<int> deleteKeys) {
    std::cout << "--- Running Test: " << testName << " ---\n";
    Tree tree;

    std::cout << "\n--- Insertion Phase ---\n";
    for (int key : insertKeys) {
        std::cout << "Operation: insert " << key << "\n";
        tree.insert(key);
        tree.print_BST();
        std::cout << "\n";
    }

     std::cout << "\n--- Control ---\n";
    //  tree.print_BST();

    std::cout << "\n--- Deletion Phase ---\n";
    for (int key : deleteKeys) {
        std::cout << "Operation: delete " << key << "\n";
        tree.deletion(key);
        tree.print_BST();
        std::cout << "\n";
    }
    std::cout << "--- Test: " << testName << " Completed ---\n\n";
}

int main() {
    const int n = 30;
    std::vector<int> keys(n);
    for (int i = 0; i < n; ++i) {
        keys[i] = i + 1;
    }

    // Case 1: Inserting a sorted sequence, deleting a random permutation
    std::vector<int> case1_insert_keys = keys; // Already sorted
    std::vector<int> case1_delete_keys = keys;
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(case1_delete_keys.begin(), case1_delete_keys.end(), std::default_random_engine(seed1));
    runTest("Case 1: Insert Sorted, Delete Random", case1_insert_keys, case1_delete_keys);

    // Case 2: Inserting a random permutation, deleting a random permutation
    std::vector<int> case2_insert_keys = keys;
    unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(case2_insert_keys.begin(), case2_insert_keys.end(), std::default_random_engine(seed2));
    std::vector<int> case2_delete_keys = keys;
    unsigned seed3 = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(case2_delete_keys.begin(), case2_delete_keys.end(), std::default_random_engine(seed3));
    runTest("Case 2: Insert Random, Delete Random", case2_insert_keys, case2_delete_keys);

    return 0;
}