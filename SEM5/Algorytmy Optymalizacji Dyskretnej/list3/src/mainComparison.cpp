#include "../include/Parser.h"
#include "../include/Dijkstra.h"
#include "../include/Dial.h"
#include "../include/RadixDijkstra.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <cmath>
#include <algorithm>

using DoubleSeconds = std::chrono::duration<double>;

// Helper function to print distances nicely
void printDistances(const std::string& algorithmName, const std::vector<long long>& distances, int limit = 10) {
    std::cout << "\n" << algorithmName << " - First " << std::min((int)distances.size(), limit) << " distances:\n";
    std::cout << std::string(60, '-') << "\n";
    for (size_t i = 0; i < distances.size() && i < limit; ++i) {
        if (distances[i] == LLONG_MAX) {
            std::cout << "  Node " << std::setw(3) << (i + 1) << ": INF\n";
        } else {
            std::cout << "  Node " << std::setw(3) << (i + 1) << ": " << distances[i] << "\n";
        }
    }
}

// Function to compare three algorithm results
bool compareResults(const std::vector<long long>& dijkstra_result,
                   const std::vector<long long>& dial_result,
                   const std::vector<long long>& radix_result) {
    bool allMatch = true;
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "ALGORITHM COMPARISON\n";
    std::cout << std::string(80, '=') << "\n";
    
    size_t maxSize = std::max({dijkstra_result.size(), dial_result.size(), radix_result.size()});
    
    for (size_t i = 0; i < maxSize; ++i) {
        long long dij_val = (i < dijkstra_result.size()) ? dijkstra_result[i] : LLONG_MAX;
        long long dial_val = (i < dial_result.size()) ? dial_result[i] : LLONG_MAX;
        long long radix_val = (i < radix_result.size()) ? radix_result[i] : LLONG_MAX;
        
        bool match = (dij_val == dial_val && dij_val == radix_val);
        
        std::string status = match ? "✓" : "✗ MISMATCH";
        std::cout << "Node " << std::setw(3) << (i + 1) << " | ";
        
        // Dijkstra
        if (dij_val == LLONG_MAX) {
            std::cout << "Dijkstra: INF     | ";
        } else {
            std::cout << "Dijkstra: " << std::setw(10) << dij_val << " | ";
        }
        
        // Dial
        if (dial_val == LLONG_MAX) {
            std::cout << "Dial: INF     | ";
        } else {
            std::cout << "Dial: " << std::setw(10) << dial_val << " | ";
        }
        
        // Radix
        if (radix_val == LLONG_MAX) {
            std::cout << "Radix: INF     ";
        } else {
            std::cout << "Radix: " << std::setw(10) << radix_val << " ";
        }
        
        std::cout << " " << status << "\n";
        
        if (!match) allMatch = false;
    }
    
    return allMatch;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " -d <graph_file.gr> [-c <max_cost>] [-ss <source_file.ss>]\n";
        std::cerr << "Example: " << argv[0] << " -d graph.gr -c 1000 -ss sources.ss\n";
        return 1;
    }

    std::string graphFile;
    std::string sourceFile;

    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-d" && i + 1 < argc) {
            graphFile = argv[++i];
        } else if (arg == "-ss" && i + 1 < argc) {
            sourceFile = argv[++i];
        }
    }

    if (graphFile.empty()) {
        std::cerr << "Error: Graph file (-d) is required.\n";
        return 1;
    }

    try {
        // Parse graph
        std::cout << "Loading graph from: " << graphFile << "\n";
        Parser parser;
        Graph graph = parser.parseGraph(graphFile);
        int maxCost = graph.max_weight;
        std::cout << "Graph loaded: " << graph.v_size << " vertices, " << graph.e_size << " edges\n";

        // Initialize solvers
        Dijkstra dijkstraSolver;
        Dial dialSolver;
        RadixDijkstra radixSolver;

        // If source file provided, use it; otherwise use first few vertices
        std::vector<int> sourceNodes;
        if (!sourceFile.empty()) {
            sourceNodes = parser.parseSSSP(sourceFile);
        } else {
            // Use first 3 vertices as example sources
            sourceNodes = {0, 1, 2};
            if (sourceNodes.back() >= graph.v_size - 1) {
                sourceNodes = {0};
            }
        }

        // Run comparison for each source
        for (int source : sourceNodes) {
            if (source < 0 || source >= graph.v_size - 1) {
                std::cerr << "Warning: Skipping invalid source " << source << "\n";
                continue;
            }

            std::cout << "\n" << std::string(80, '=') << "\n";
            std::cout << "RUNNING ALGORITHMS FOR SOURCE: " << source << "\n";
            std::cout << std::string(80, '=') << "\n";

            // Run Dijkstra
            std::cout << "\n[1/3] Running Dijkstra's Algorithm...\n";
            auto start = std::chrono::high_resolution_clock::now();
            auto dijkstra_result = dijkstraSolver.findAllPaths(graph, source);
            auto end = std::chrono::high_resolution_clock::now();
            DoubleSeconds dijkstra_time = end - start;
            std::cout << "Time: " << std::fixed << std::setprecision(6) << dijkstra_time.count() << " seconds\n";
            printDistances("Dijkstra", dijkstra_result);

            // Run Dial
            std::cout << "\n[2/3] Running Dial's Algorithm (maxCost=" << maxCost << ")...\n";
            start = std::chrono::high_resolution_clock::now();
            auto dial_result = dialSolver.findAllPaths(graph, source, maxCost);
            end = std::chrono::high_resolution_clock::now();
            DoubleSeconds dial_time = end - start;
            std::cout << "Time: " << std::fixed << std::setprecision(6) << dial_time.count() << " seconds\n";
            printDistances("Dial", dial_result);

            // Run Radix Dijkstra
            std::cout << "\n[3/3] Running Radix Heap Dijkstra (maxCost=" << maxCost << ")...\n";
            start = std::chrono::high_resolution_clock::now();
            auto radix_result = radixSolver.findAllPaths(graph, source, maxCost);
            end = std::chrono::high_resolution_clock::now();
            DoubleSeconds radix_time = end - start;
            std::cout << "Time: " << std::fixed << std::setprecision(6) << radix_time.count() << " seconds\n";
            printDistances("Radix Heap", radix_result);

            // Compare results
            bool allMatch = compareResults(dijkstra_result, dial_result, radix_result);
            
            std::cout << "\n" << std::string(80, '=') << "\n";
            if (allMatch) {
                std::cout << "✓ ALL ALGORITHMS PRODUCED IDENTICAL RESULTS!\n";
            } else {
                std::cout << "✗ ALGORITHMS PRODUCED DIFFERENT RESULTS - CHECK FOR BUGS!\n";
            }
            std::cout << std::string(80, '=') << "\n";

            // Time comparison summary
            std::cout << "\nTIME COMPARISON:\n";
            std::cout << "  Dijkstra:    " << std::fixed << std::setprecision(6) << dijkstra_time.count() << "s\n";
            std::cout << "  Dial:        " << std::fixed << std::setprecision(6) << dial_time.count() << "s";
            if (dial_time.count() > 0) {
                std::cout << " (" << std::setprecision(2) << (dijkstra_time.count() / dial_time.count()) << "x vs Dijkstra)\n";
            } else {
                std::cout << "\n";
            }
            std::cout << "  Radix Heap:  " << std::fixed << std::setprecision(6) << radix_time.count() << "s";
            if (radix_time.count() > 0) {
                std::cout << " (" << std::setprecision(2) << (dijkstra_time.count() / radix_time.count()) << "x vs Dijkstra)\n";
            } else {
                std::cout << "\n";
            }
        }

        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << "COMPARISON COMPLETE\n";
        std::cout << std::string(80, '=') << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
