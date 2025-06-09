#include "../Header/BinomialHeap.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

void runExperiment(int n, int experimentId) {
    BinomialHeap H1, H2;

    std::srand(std::time(nullptr) + experimentId);
    for (int i = 0; i < n; ++i) {
        H1.insert(std::rand());
        H2.insert(std::rand());
    }

    H1.merge(H2);

    std::vector<int> extracted;
    std::vector<int> comparisonsPerExtract;

    for (int i = 0; i < 2 * n; ++i) {
        H1.resetComparisons();
        int min = H1.extractMin();
        int comps = H1.getComparisons();

        if (min == INT_MIN) {
            std::cerr << "Error: extracted INT_MIN\n";
            return;
        }

        comparisonsPerExtract.push_back(comps);
        extracted.push_back(min);
    }

    // Check if sorted
    for (size_t i = 1; i < extracted.size(); ++i) {
        if (extracted[i - 1] > extracted[i]) {
            std::cerr << "Error: Extracted sequence is not sorted.\n";
            return;
        }
    }

    // Save to file
    std::ofstream file("experiment_" + std::to_string(experimentId) + ".csv");
    for (size_t i = 0; i < comparisonsPerExtract.size(); ++i) {
        file << i + 1 << "," << comparisonsPerExtract[i] << "\n";
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
