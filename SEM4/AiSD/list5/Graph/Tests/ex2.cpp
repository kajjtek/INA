#include <iostream>
#include <limits>
#include "../Headers/DSU.h"
#include "../Headers/GreedyAlgorithms.h"
#include "../Headers/Node.h"
#include "../Headers/Graph.h"
#include <random>
#include <iomanip>

int main() {
    std::cout << "Rozpoczynam analizę eksperymentalną..." << std::endl;

    // Parametry eksperymentu
    const int MIN_NODES = 100;         // Minimalna liczba wierzchołków
    const int MAX_NODES = 1000;        // Maksymalna liczba wierzchołków
    const int NODE_STEP = 100;         // Krok zwiększania liczby wierzchołków
    const int NUM_TRIALS_PER_N = 20;   // Liczba powtórzeń dla każdego rozmiaru drzewa

    std::random_device rd;   // Urządzenie do generowania prawdziwie losowych wartości (do inicjalizacji)
    std::mt19937 rng(rd());

    // Nagłówek dla wyjścia w formacie CSV
    std::cout << "N_Nodes,Min_Rounds,Max_Rounds,Avg_Rounds" << std::endl;
    GreedyAlgorithms util;

    // Główna pętla eksperymentalna
    for (int n = MIN_NODES; n <= MAX_NODES; n += NODE_STEP) {
        // Zmienne do zbierania wyników dla bieżącej liczby wierzchołków 'n'
        int min_rounds_for_n = std::numeric_limits<int>::max(); // Inicjalizacja na maksymalną wartość int
        int max_rounds_for_n = 0;                               // Inicjalizacja na 0
        long long sum_rounds_for_n = 0;                         // Użyj long long, aby uniknąć przepełnienia sumy

        // Pętla wykonująca wiele prób dla bieżącej liczby wierzchołków 'n'
        for (int trial = 0; trial < NUM_TRIALS_PER_N; ++trial) {
            // 1. Generuj losowy graf (w tym przypadku już drzewo)
            // Obiekt `randomGraph` zostanie zniszczony na końcu tej iteracji pętli wewnętrznej,
            // a jego destruktor zwolni zaalokowane Node*.
            Graph randomGraph;
            randomGraph.createRandomFullGraph(n);
            
            // 2. Wygeneruj MST z `randomGraph` za pomocą algorytmu Pryma.
            // Ważne: Zwrócony obiekt `mst` będzie zawierał *nowe* obiekty Node*,
            // które są kopiami struktury z `randomGraph`.
            Graph mst = util.prism(randomGraph);
            
            // Sprawdzenie, czy MST zostało poprawnie utworzone i nie jest puste
            if (mst.allNodes.empty()) {
                std::cerr << "Ostrzeżenie: MST jest puste dla N=" << n << ", próba=" << trial << ". Pomijam." << std::endl;
                continue; // Pomiń tę próbę, jeśli MST jest puste (nie powinno się zdarzyć dla n > 0)
            }

            // 3. Wybierz losowy wierzchołek startowy z *wygenerowanego MST*
            // std::uniform_int_distribution<> node_idx_dist(0, mst.allNodes.size() - 1);
            // Node* randomStartNode = mst.allNodes[node_idx_dist(rng)];

            // 4. Uruchom symulację propagacji informacji na MST
            // Funkcja informationPassing powinna zwrócić maksymalną liczbę rund.
            int rounds_needed = util.informationPassing(mst);
            
            // 5. Zbieraj wyniki z bieżącej próby
            min_rounds_for_n = std::min(min_rounds_for_n, rounds_needed);
            max_rounds_for_n = std::max(max_rounds_for_n, rounds_needed);
            sum_rounds_for_n += rounds_needed;

            // Obiekty `mst` i `randomGraph` (i ich zawartość Node*) zostaną automatycznie zwolnione
            // po zakończeniu tej iteracji pętli wewnętrznej dzięki destruktorom klasy Graph.
        }

        // Oblicz średnią liczbę rund dla bieżącej liczby wierzchołków 'n'
        double avg_rounds_for_n = static_cast<double>(sum_rounds_for_n) / NUM_TRIALS_PER_N;

        // Wypisz wyniki w formacie CSV (N_Nodes,Min_Rounds,Max_Rounds,Avg_Rounds)
        // std::fixed i std::setprecision(2) służą do formatowania liczby zmiennoprzecinkowej.
        std::cout << n << ","
                  << min_rounds_for_n << ","
                  << max_rounds_for_n << ","
                  << std::fixed << std::setprecision(2) << avg_rounds_for_n
                  << std::endl;
    }

    std::cout << "Analiza zakończona. Wyniki zostały wypisane na standardowe wyjście (format CSV)." << std::endl;
    std::cout << "Możesz przekierować to wyjście do pliku, np. './your_program > results.csv'" << std::endl;

    return 0;
}