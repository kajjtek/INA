#include "../Header/BinomialHeap.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

void runExperiment(int n, int experimentId) {
    BinomialHeap H1, H2;

    std::srand(std::time(nullptr) + experimentId);

    std::vector<int> comparisonsLog;  // Store comparisons in order

    // Insert n elements into H1, log comparisons each insert
    for (int i = 0; i < n; ++i) {
        H1.resetComparisons();
        H1.insert(std::rand());
        comparisonsLog.push_back(H1.getComparisons());
    }

    // Insert n elements into H2, log comparisons each insert
    for (int i = 0; i < n; ++i) {
        H2.resetComparisons();
        H2.insert(std::rand());
        comparisonsLog.push_back(H2.getComparisons());
    }

    // Heap-union operation (merge)
    H1.resetComparisons();
    H1.merge(H2);
    comparisonsLog.push_back(H1.getComparisons());

    // Now 2*n extractMin operations with comparisons logged
    for (int i = 0; i < 2 * n; ++i) {
        H1.resetComparisons();
        int minVal = H1.extractMin();
        if (minVal == INT_MIN) {
            std::cerr << "Error: extracted INT_MIN\n";
            return;
        }
        comparisonsLog.push_back(H1.getComparisons());
    }

    // Save to single CSV file
    std::ofstream file("experiment_" + std::to_string(experimentId) + ".csv");
    for (size_t i = 0; i < comparisonsLog.size(); ++i) {
        file << (i + 1) << "," << comparisonsLog[i] << "\n";
    }
    file.close();

    std::cout << "Experiment " << experimentId << " done.\n";
}


void runAverageCostExperiment() {
    std::ofstream file("average_cost.csv");
    for (int n = 100; n <= 10000; n += 100) {
        long long totalComparisons = 0;
        for (int t = 0; t < 5; ++t) {
            BinomialHeap H1, H2;
            std::srand(std::time(nullptr) + t + n);
            for (int i = 0; i < n; ++i) {
                H1.insert(std::rand());
                H2.insert(std::rand());
            }

            H1.merge(H2);
            for (int i = 0; i < 2 * n; ++i) {
                H1.resetComparisons();
                H1.extractMin();
                totalComparisons += H1.getComparisons();
            }
        }
        double avgCost = static_cast<double>(totalComparisons) / (2 * n * 5);
        file << n << "," << avgCost << "\n";
        std::cout << "n = " << n << " done.\n";
    }
    file.close();
}

int main() {
    // 5 experiments for n = 500
    for (int i = 0; i < 5; ++i) {
        runExperiment(500, i);
    }

    // Average cost experiment
    runAverageCostExperiment();

    return 0;
}
