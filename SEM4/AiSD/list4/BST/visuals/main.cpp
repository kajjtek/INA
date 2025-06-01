#include "../Headers/Tree.h"
#include <iostream>
#include <vector>
#include <numeric>   // For std::iota
#include <random>    // For std::random_device, std::mt19937, std::shuffle
#include <algorithm> // For std::shuffle
#include <chrono>    // For timing (though not explicitly requested, good practice)
#include <fstream>   // For file output

// Structure to hold results for a single N
struct ExperimentResult {
    int n_value;
    std::string scenario;

    double avg_insert_comparisons;
    long long max_insert_comparisons;
    double avg_insert_pointer_ops;
    long long max_insert_pointer_ops;
    double avg_insert_height;
    long long max_insert_height;

    double avg_delete_comparisons;
    long long max_delete_comparisons;
    double avg_delete_pointer_ops;
    long long max_delete_pointer_ops;
    double avg_delete_height;
    long long max_delete_height;
};

// Function to run a single experiment for a given N and scenario
ExperimentResult run_single_experiment(int n, bool ascending_insert) {
    ExperimentResult res;
    res.n_value = n;
    res.scenario = ascending_insert ? "Ascending Insert & Random Delete" : "Random Insert & Random Delete";

    std::vector<long long> all_insert_comparisons;
    std::vector<long long> all_insert_pointer_ops;
    std::vector<long long> all_insert_heights;

    std::vector<long long> all_delete_comparisons;
    std::vector<long long> all_delete_pointer_ops;
    std::vector<long long> all_delete_heights;

    const int NUM_RUNS = 20;

    std::random_device rd;
    std::mt19937 g(rd());

    for (int run = 0; run < NUM_RUNS; ++run) {
        Tree tree; // A new tree for each run

        // Generate keys for insertion
        std::vector<int> insert_keys(n);
        if (ascending_insert) {
            std::iota(insert_keys.begin(), insert_keys.end(), 1); // 1, 2, ..., n
        } else {
            std::iota(insert_keys.begin(), insert_keys.end(), 1);
            std::shuffle(insert_keys.begin(), insert_keys.end(), g); // Random permutation
        }

        // Insertion Phase
        for (int key : insert_keys) {
            Tree::reset_counters(); // Reset counters for current operation
            tree.insert(key);
            all_insert_comparisons.push_back(Tree::comparisons);
            all_insert_pointer_ops.push_back(Tree::pointer_operations);
            all_insert_heights.push_back(tree.height()); // tree.height() updates Tree::current_height
        }

        

        // Generate keys for deletion (random permutation of inserted keys)
        std::vector<int> delete_keys = insert_keys; // Copy for deletion
        std::shuffle(delete_keys.begin(), delete_keys.end(), g); // Random permutation for deletion

        // Deletion Phase
        for (int key : delete_keys) {
            Tree::reset_counters(); // Reset counters for current operation
            tree.deletion(key);
            all_delete_comparisons.push_back(Tree::comparisons);
            all_delete_pointer_ops.push_back(Tree::pointer_operations);
            all_delete_heights.push_back(tree.height()); // tree.height() updates Tree::current_height
        }
    }

    // Calculate overall averages and maximums
    auto calculate_avg = [](const std::vector<long long>& vec) {
        if (vec.empty()) return 0.0;
        return static_cast<double>(std::accumulate(vec.begin(), vec.end(), 0LL)) / vec.size();
    };

    auto calculate_max = [](const std::vector<long long>& vec) {
        if (vec.empty()) return 0LL;
        return *std::max_element(vec.begin(), vec.end());
    };

    res.avg_insert_comparisons = calculate_avg(all_insert_comparisons);
    res.max_insert_comparisons = calculate_max(all_insert_comparisons);
    res.avg_insert_pointer_ops = calculate_avg(all_insert_pointer_ops);
    res.max_insert_pointer_ops = calculate_max(all_insert_pointer_ops);
    res.avg_insert_height = calculate_avg(all_insert_heights);
    res.max_insert_height = calculate_max(all_insert_heights);

    res.avg_delete_comparisons = calculate_avg(all_delete_comparisons);
    res.max_delete_comparisons = calculate_max(all_delete_comparisons);
    res.avg_delete_pointer_ops = calculate_avg(all_delete_pointer_ops);
    res.max_delete_pointer_ops = calculate_max(all_delete_pointer_ops);
    res.avg_delete_height = calculate_avg(all_delete_heights);
    res.max_delete_height = calculate_max(all_delete_heights);

    return res;
}

int main() {
    std::ofstream csv_file("experiment_results.csv");
    if (!csv_file.is_open()) {
        std::cerr << "Error opening CSV file!" << std::endl;
        return 1;
    }

    // Write CSV header
    csv_file << "N,Scenario,"
             << "Avg_Insert_Comparisons,Max_Insert_Comparisons,"
             << "Avg_Insert_Pointer_Ops,Max_Insert_Pointer_Ops,"
             << "Avg_Insert_Height,Max_Insert_Height,"
             << "Avg_Delete_Comparisons,Max_Delete_Comparisons,"
             << "Avg_Delete_Pointer_Ops,Max_Delete_Pointer_Ops,"
             << "Avg_Delete_Height,Max_Delete_Height\n";

    for (int n = 10000; n <= 100000; n += 10000) {
        std::cout << "Running experiments for N = " << n << std::endl;

        // Ascending Insert & Random Delete
        ExperimentResult res_asc = run_single_experiment(n, true);
        csv_file << res_asc.n_value << ","
                 << res_asc.scenario << ","
                 << res_asc.avg_insert_comparisons << "," << res_asc.max_insert_comparisons << ","
                 << res_asc.avg_insert_pointer_ops << "," << res_asc.max_insert_pointer_ops << ","
                 << res_asc.avg_insert_height << "," << res_asc.max_insert_height << ","
                 << res_asc.avg_delete_comparisons << "," << res_asc.max_delete_comparisons << ","
                 << res_asc.avg_delete_pointer_ops << "," << res_asc.max_delete_pointer_ops << ","
                 << res_asc.avg_delete_height << "," << res_asc.max_delete_height << "\n";

        // Random Insert & Random Delete
        ExperimentResult res_rand = run_single_experiment(n, false);
        csv_file << res_rand.n_value << ","
                 << res_rand.scenario << ","
                 << res_rand.avg_insert_comparisons << "," << res_rand.max_insert_comparisons << ","
                 << res_rand.avg_insert_pointer_ops << "," << res_rand.max_insert_pointer_ops << ","
                 << res_rand.avg_insert_height << "," << res_rand.max_insert_height << ","
                 << res_rand.avg_delete_comparisons << "," << res_rand.max_delete_comparisons << ","
                 << res_rand.avg_delete_pointer_ops << "," << res_rand.max_delete_pointer_ops << ","
                 << res_rand.avg_delete_height << "," << res_rand.max_delete_height << "\n";
    }

    csv_file.close();
    std::cout << "Experiment results saved to experiment_results.csv" << std::endl;

    return 0;
}