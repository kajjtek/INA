#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <climits>
#include <iomanip>
#include <chrono>
#include "../Header/Node.h"
#include "../Header/BinomialHeap.h"


void runSingleNExperiment(int nVal, int experimentRunId, std::ofstream& dataFile) {
    BinomialHeap H1, H2, H;
    std::vector<long long> comparisonsPerOpHistory;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 1000000);

    for (int i = 0; i < nVal; ++i) {
        int val = distrib(gen);
        H1.heapInsert(val);
        comparisonsPerOpHistory.push_back(H1.getComparisonCount());
        H1.resetComparisonCount();
    }

    for (int i = 0; i < nVal; ++i) {
        int val = distrib(gen);
        H2.heapInsert(val);
        comparisonsPerOpHistory.push_back(H2.getComparisonCount());
        H2.resetComparisonCount();
    }

    H.heapUnion(H1);
    comparisonsPerOpHistory.push_back(H.getComparisonCount());
    H.resetComparisonCount();

    H.heapUnion(H2);
    comparisonsPerOpHistory.push_back(H.getComparisonCount());
    H.resetComparisonCount();

    std::vector<int> extractedElements;
    for (int i = 0; i < 2 * nVal; ++i) {
        Node* minNode = H.extractMin();
        if (minNode) {
            extractedElements.push_back(minNode->key);
            comparisonsPerOpHistory.push_back(H.getComparisonCount());
            H.resetComparisonCount();
            delete minNode;
        } else {
            std::cerr << "Błąd: Próbowano wykonać Extract-Min na pustym kopcu przed 2n operacjami dla n="
                      << nVal << ", eksperyment=" << experimentRunId << std::endl;
            break;
        }
    }

    bool isSorted = std::is_sorted(extractedElements.begin(), extractedElements.end());
    if (!isSorted) {
        std::cerr << "Błąd: Wyodrębnione elementy nie są posortowane dla n=" << nVal
                  << ", eksperyment=" << experimentRunId << std::endl;
    }
    if (H.getHead() != nullptr) {
        std::cerr << "Błąd: Kopiec nie jest pusty po 2n ekstrakcjach dla n=" << nVal
                  << ", eksperyment=" << experimentRunId << std::endl;
    }

    for (size_t i = 0; i < comparisonsPerOpHistory.size(); ++i) {
        dataFile << experimentRunId << "," << i << "," << comparisonsPerOpHistory[i] << std::endl;
    }
}

void runNvsAverageCostExperiment(int startN, int endN, int stepN, std::ofstream& dataFile) {
    std::random_device rd;
    std::mt19937 gen(rd());

    dataFile << "n,average_cost" << std::endl;

    for (int n = startN; n <= endN; n += stepN) {
        long long totalComparisonsForN = 0;
        int numTrials = 3;

        for (int trial = 0; trial < numTrials; ++trial) {
            BinomialHeap H1, H2, H;
            std::uniform_int_distribution<> distrib(1, 1000000);

            for (int i = 0; i < n; ++i) {
                H1.heapInsert(distrib(gen));
            }
            for (int i = 0; i < n; ++i) {
                H2.heapInsert(distrib(gen));
            }

            H.heapUnion(H1);
            H.heapUnion(H2);

            for (int i = 0; i < 2 * n; ++i) {
                Node* minNode = H.extractMin();
                if (minNode) {
                    delete minNode;
                }
            }
            totalComparisonsForN += H.getTotalComparisonCount();
        }
        double avgCostPerInitialN = static_cast<double>(totalComparisonsForN) / (numTrials * n);

        dataFile << n << "," << std::fixed << std::setprecision(6) << avgCostPerInitialN << std::endl;
    }
}


int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::ofstream exp1Data("comparisons_per_operation_n500.csv");
    if (!exp1Data.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku comparisons_per_operation_n500.csv" << std::endl;
        return 1;
    }
    exp1Data << "experiment_id,operation_index,comparisons_count" << std::endl;

    std::cout << "Przeprowadzanie eksperymentow dla n = 500 (5 powtorzen)..." << std::endl;
    for (int i = 0; i < 5; ++i) {
        runSingleNExperiment(500, i + 1, exp1Data);
    }
    exp1Data.close();
    std::cout << "Dane dla eksperymentu n=500 zapisane do comparisons_per_operation_n500.csv" << std::endl;
    std::cout << "---" << std::endl;

    std::ofstream exp2Data("n_vs_average_cost.csv");
    if (!exp2Data.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku n_vs_average_cost.csv" << std::endl;
        return 1;
    }

    std::cout << "\nPrzeprowadzanie eksperymentow dla n od 100 do 10000 (krok 100)..." << std::endl;
    runNvsAverageCostExperiment(100, 10000, 100, exp2Data);
    exp2Data.close();
    std::cout << "Dane dla eksperymentu zaleznosci n od sredniego kosztu zapisane do n_vs_average_cost.csv" << std::endl;
    std::cout << "---" << std::endl;

    return 0;
}