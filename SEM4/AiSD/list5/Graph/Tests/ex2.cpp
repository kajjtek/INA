#include <iostream>
#include <limits>
#include <fstream> // Required for file operations
#include "../Headers/DSU.h"
#include "../Headers/GreedyAlgorithms.h"
#include "../Headers/Node.h"
#include "../Headers/Graph.h"
#include <random>
#include <iomanip>

int main() {
    std::cout << "Rozpoczynam analizę eksperymentalną..." << std::endl;

    // Experiment parameters
    const int MIN_NODES = 100;         // Minimum number of nodes
    const int MAX_NODES = 1000;        // Maximum number of nodes
    const int NODE_STEP = 100;         // Step for increasing node count
    const int NUM_TRIALS_PER_N = 20;   // Number of trials for each tree size

    std::random_device rd;   // Device for generating truly random values (for initialization)
    std::mt19937 rng(rd());

    // --- File output setup ---
    // Open a file stream for writing
    std::ofstream outputFile("information_passing_results.csv");

    // Check if the file was opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Błąd: Nie można otworzyć pliku information_passing_results.csv do zapisu." << std::endl;
        return 1; // Indicate an error
    }

    // Write the CSV header to the file
    outputFile << "N_Nodes,Min_Rounds,Max_Rounds,Avg_Rounds" << std::endl;
    // Also print to console for immediate feedback
    std::cout << "N_Nodes,Min_Rounds,Max_Rounds,Avg_Rounds" << std::endl;
    // --- End file output setup ---

    GreedyAlgorithms util;

    // Main experimental loop
    for (int n = MIN_NODES; n <= MAX_NODES; n += NODE_STEP) {
        // Variables for collecting results for the current number of nodes 'n'
        int min_rounds_for_n = std::numeric_limits<int>::max(); // Initialize to maximum int value
        int max_rounds_for_n = 0;                               // Initialize to 0
        long long sum_rounds_for_n = 0;                         // Use long long to prevent sum overflow

        // Loop performing multiple trials for the current number of nodes 'n'
        for (int trial = 0; trial < NUM_TRIALS_PER_N; ++trial) {
            // 1. Generate a random graph (in this case, already a tree)
            // The `randomGraph` object will be destroyed at the end of this inner loop iteration,
            // and its destructor will deallocate the allocated Node*s.
            Graph randomGraph;
            randomGraph.createRandomFullGraph(n);

            // 2. Generate an MST from `randomGraph` using Prim's algorithm.
            // Important: The returned `mst` object will contain *new* Node* objects,
            // which are copies of the structure from `randomGraph`.
            Graph mst = util.prism(randomGraph);

            // Check if MST was correctly created and is not empty
            if (mst.allNodes.empty()) {
                std::cerr << "Ostrzeżenie: MST jest puste dla N=" << n << ", próba=" << trial << ". Pomijam." << std::endl;
                continue; // Skip this trial if MST is empty (shouldn't happen for n > 0)
            }

            // 3. Choose a random starting node from the *generated MST*
            // This part was commented out in your original code, if you want to use it:
            // std::uniform_int_distribution<> node_idx_dist(0, mst.allNodes.size() - 1);
            // Node* randomStartNode = mst.allNodes[node_idx_dist(rng)];

            // 4. Run the information passing simulation on the MST
            // The informationPassing function should return the maximum number of rounds.
            int rounds_needed = util.informationPassing(mst);

            // 5. Collect results from the current trial
            min_rounds_for_n = std::min(min_rounds_for_n, rounds_needed);
            max_rounds_for_n = std::max(max_rounds_for_n, rounds_needed);
            sum_rounds_for_n += rounds_needed;

            // `mst` and `randomGraph` objects (and their Node* contents) will be automatically
            // deallocated at the end of this inner loop iteration thanks to the Graph class destructors.
        }

        // Calculate the average number of rounds for the current number of nodes 'n'
        double avg_rounds_for_n = static_cast<double>(sum_rounds_for_n) / NUM_TRIALS_PER_N;

        // Write the results to the file in CSV format (N_Nodes,Min_Rounds,Max_Rounds,Avg_Rounds)
        // std::fixed and std::setprecision(2) are used for floating-point number formatting.
        outputFile << n << ","
                   << min_rounds_for_n << ","
                   << max_rounds_for_n << ","
                   << std::fixed << std::setprecision(2) << avg_rounds_for_n
                   << std::endl;

        // Also print to console for real-time progress monitoring
        std::cout << n << ","
                  << min_rounds_for_n << ","
                  << max_rounds_for_n << ","
                  << std::fixed << std::setprecision(2) << avg_rounds_for_n
                  << std::endl;
    }

    // --- File output cleanup ---
    outputFile.close(); // Close the output file when done
    // --- End file output cleanup ---

    std::cout << "Analiza zakończona. Wyniki zostały zapisane do pliku information_passing_results.csv" << std::endl;
    std::cout << "Wyniki są również wypisywane na standardowe wyjście." << std::endl;

    return 0;
}