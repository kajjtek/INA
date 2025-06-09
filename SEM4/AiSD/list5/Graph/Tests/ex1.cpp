#include <iostream>
#include <chrono>
#include <fstream>
#include "../Headers/GreedyAlgorithms.h"

using namespace std;
using namespace std::chrono;

int main() {
    int nMin = 100;
    int nMax = 2000;
    int step = 100;
    int rep = 20;

    ofstream csvFile("results.csv");
    if (!csvFile.is_open()) {
        cerr << "Failed to open CSV file." << endl;
        return 1;
    }

    // Write CSV header
    csvFile << "n,AvgPrim(ms),AvgKruskal(ms)\n";
    cout << "n AvgPrim(ms) AvgKruskal(ms)" << endl;

    for (int n = nMin; n <= nMax; n += step) {
        double totalPrimTime = 0;
        double totalKruskalTime = 0;

        for (int i = 0; i < rep; ++i) {
            Graph g;
            g.createRandomFullGraph(n);

            GreedyAlgorithms greedy;

            // Prim's Algorithm Timing
            auto startPrim = high_resolution_clock::now();
            Graph mstPrim = greedy.prism(g);
            auto endPrim = high_resolution_clock::now();
            totalPrimTime += duration<double, std::milli>(endPrim - startPrim).count();
            mstPrim.clean();

            // Kruskal's Algorithm Timing
            auto startKruskal = high_resolution_clock::now();
            Graph mstKruskal = greedy.Kruskal(g);
            auto endKruskal = high_resolution_clock::now();
            totalKruskalTime += duration<double, std::milli>(endKruskal - startKruskal).count();
            mstKruskal.clean();

            g.clean();
        }

        double avgPrim = totalPrimTime / rep;
        double avgKruskal = totalKruskalTime / rep;

        // Print to console
        cout << n << " " << avgPrim << " " << avgKruskal << endl;

        // Write to CSV
        csvFile << n << "," << avgPrim << "," << avgKruskal << "\n";
    }

    csvFile.close();
    cout << "Results written to results.csv" << endl;

    return 0;
}
