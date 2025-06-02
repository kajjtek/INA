#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <string>
#include "../Headers/RBTree.h"

using namespace std;

struct Metrics {
    long long totalComparisons = 0;
    long long totalPointerOps = 0;
    long long totalHeight = 0;
    long long maxComparisons = 0;
    long long maxPointerOps = 0;
    int maxHeight = 0;
    int count = 0;

    void update(long long comp, long long ptr, int height) {
        totalComparisons += comp;
        totalPointerOps += ptr;
        totalHeight += height;
        maxComparisons = max(maxComparisons, comp);
        maxPointerOps = max(maxPointerOps, ptr);
        maxHeight = max(maxHeight, height);
        count++;
    }

    void reset() {
        totalComparisons = totalPointerOps = totalHeight = 0;
        maxComparisons = maxPointerOps = maxHeight = 0;
        count = 0;
    }
};

void runScenario(ofstream& csv, int n, bool sortedInsert, const string& scenarioLabel) {
    random_device rd;
    mt19937 rng(rd());
    Metrics metrics;

    for (int test = 0; test < 20; ++test) {
        vector<int> values(n);
        iota(values.begin(), values.end(), 1);

        if (!sortedInsert)
            shuffle(values.begin(), values.end(), rng);

        RBTree tree;

        // Insert
        for (int key : values) {
            RBTree::reset_counters();
            tree.insert(key);
            metrics.update(RBTree::key_comparisons, RBTree::pointer_operations, tree.height());
        }

        // Prepare deletion permutation
        shuffle(values.begin(), values.end(), rng);

        // Delete
        for (int key : values) {
            RBTree::reset_counters();
            tree.deletion(key);
            metrics.update(RBTree::key_comparisons, RBTree::pointer_operations, tree.height());
        }
    }

    // Write to CSV
    csv << n << ","
        << scenarioLabel << ","
        << (double)metrics.totalComparisons / metrics.count << ","
        << metrics.maxComparisons << ","
        << (double)metrics.totalPointerOps / metrics.count << ","
        << metrics.maxPointerOps << ","
        << (double)metrics.totalHeight / metrics.count << ","
        << metrics.maxHeight << "\n";
}

int main() {
    ofstream csv("results.csv");
    if (!csv.is_open()) {
        cerr << "Nie można otworzyć pliku results.csv\n";
        return 1;
    }

    // Nagłówek CSV
    csv << "n,scenario,avg_comparisons,max_comparisons,avg_pointers,max_pointers,avg_height,max_height\n";

    for (int n = 10000; n <= 100000; n += 10000) {
        runScenario(csv, n, true, "sorted_insert_random_delete");
        runScenario(csv, n, false, "random_insert_random_delete");
    }

    csv.close();
    cout << "Eksperymenty zakończone. Wyniki zapisane do results.csv\n";
    return 0;
}
