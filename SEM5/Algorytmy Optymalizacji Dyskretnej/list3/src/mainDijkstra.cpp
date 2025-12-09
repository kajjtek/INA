#include "../include/Parser.h"
#include "../include/Dijkstra.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <map>
#include <limits>
#include <stdexcept>

// Typ do mierzenia czasu (sekundy)
using DoubleSeconds = std::chrono::duration<double>;

// Funkcja pomocnicza do wyświetlania użycia programu (pozostawiam, aby pomóc użytkownikowi w razie błędu)
void printUsage(const std::string& appName) {
    std::cerr << "Uzycie: " << appName << " -d <plik_grafu.gr> [-ss <plik_zrodel.ss>] [-oss <plik_wynikow_ss.res>] [-p2p <plik_par.p2p>] [-op2p <plik_wynikow_p2p.res>]\n";
    std::cerr << "Wymagane:\n";
    std::cerr << "  -d <plik_grafu.gr>     Sciezka do pliku grafu w formacie DIMACS.\n";
    std::cerr << "Opcjonalnie:\n";
    std::cerr << "  -ss <plik_zrodel.ss>   Sciezka do pliku zrodlowego SSSP.\n";
    std::cerr << "  -oss <plik_wynikow_ss.res> Sciezka do pliku wyjsciowego dla wynikow SSSP (zawiera tylko czas 't').\n";
    std::cerr << "  -p2p <plik_par.p2p>    Sciezka do pliku par P2P.\n";
    std::cerr << "  -op2p <plik_wynikow_p2p.res> Sciezka do pliku wyjsciowego dla wynikow P2P (zawiera czas 't' i wyniki 'd').\n";
    std::cerr << "Program musi byc wywolany z jednym zestawem (SSSP lub P2P).\n";
}

/**
 * @brief Zapisuje pełne wyniki (dystanse) dla SSSP do pliku weryfikacyjnego.
 * @param outputFilename Scieżka do pliku wyjsciowego (np. .res.dist).
 * @param sourceNode Węzeł źródłowy.
 * @param distances Wektor odległości.
 */
void saveSSSPDistances(
    const std::string& outputFilename, 
    int sourceNode, 
    const std::vector<long long>& distances
) {
    // Otwarcie pliku w trybie append, aby dodawać wyniki kolejnych źródeł (dla weryfikacji)
    std::ofstream outFile(outputFilename, std::ios_base::app); 
    if (!outFile.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku weryfikacyjnego: " << outputFilename << std::endl;
        return;
    }

    outFile << "c Wyniki dla algorytmu Dijkstry, zrodlo: " << sourceNode << "\n";
    
    // Zapis dystansów (DIMACS format: d WIERZCHOLEK DYSTANS)
    // Wierzchołki są 1-bazowe (indeks 1 do N)
    for (size_t i = 1; i < distances.size(); ++i) {
        if (distances[i] != std::numeric_limits<long long>::max()) {
            outFile << "d " << i << " " << distances[i] << "\n";
        }
    }
}

/**
 * @brief Zapisuje wyniki P2P (dystanse i czas).
 * @param outputFilename Scieżka do pliku wyjsciowego (.res).
 * @param queries Wektor zapytań P2P.
 * @param results Wektor wyników P2P (para: target, distance).
 * @param executionTime Całkowity czas wykonania.
 */
void saveP2PResult(
    const std::string& outputFilename, 
    const std::vector<std::pair<int, int>>& queries, 
    const std::vector<std::pair<int, long long>>& results,
    double executionTime
) {
    // Zapisujemy tylko raz, nadpisując plik (dla P2P)
    std::ofstream outFile(outputFilename); 
    if (!outFile.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku wyjsciowego P2P: " + outputFilename);
    }
    
    // Zapis czasu wykonania (DIMACS format: t SECONDS)
    outFile << std::fixed << std::setprecision(6) << "t " << executionTime << "\n";

    // Zapis dystansów dla każdej pary (DIMACS format: d ZRODLO UJSCIE DYSTANS)
    for (size_t i = 0; i < queries.size(); ++i) {
        int source = queries[i].first;
        int target = queries[i].second;
        long long distance = results[i].second;

        // Zgodnie z formatem, zapisujemy tylko osiągalne ścieżki
        if (distance != std::numeric_limits<long long>::max()) {
            outFile << "d " << source << " " << target << " " << distance << "\n";
        }
    }
}


int main(int argc, char* argv[]) {
    // 1. Parsowanie argumentów linii poleceń
    std::cout<<"JEstem";
    std::string graphFile, ssFile, p2pFile, ssOutFile, p2pOutFile;
    
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-d" && i + 1 < argc) {
            graphFile = argv[++i];
        } else if (std::string(argv[i]) == "-ss" && i + 1 < argc) {
            ssFile = argv[++i];
        } else if (std::string(argv[i]) == "-oss" && i + 1 < argc) {
            ssOutFile = argv[++i];
        } else if (std::string(argv[i]) == "-p2p" && i + 1 < argc) {
            p2pFile = argv[++i];
        } else if (std::string(argv[i]) == "-op2p" && i + 1 < argc) {
            p2pOutFile = argv[++i];
        } else {
            std::cerr << "Nieznany argument lub brak wartosci: " << argv[i] << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    // 2. Walidacja argumentów
    if (graphFile.empty()) {
        std::cerr << "Blad: Wymagany plik grafu (-d).\n";
        printUsage(argv[0]);
        return 1;
    }

    bool isSSSP = !ssFile.empty();
    bool isP2P = !p2pFile.empty();

    if ((isSSSP && isP2P) || (!isSSSP && !isP2P)) {
        std::cerr << "Blad: Musisz wybrac albo tryb SSSP (-ss) albo P2P (-p2p).\n";
        printUsage(argv[0]);
        return 1;
    }

    if (isSSSP && ssOutFile.empty()) {
        std::cerr << "Blad: W trybie SSSP wymagany plik wyjsciowy (-oss).\n";
        return 1;
    }
     if (isP2P && p2pOutFile.empty()) {
        std::cerr << "Blad: W trybie P2P wymagany plik wyjsciowy (-op2p).\n";
        return 1;
    }


    try {
        Parser parser;
        Dijkstra dijkstraSolver;

        // Parsowanie grafu: niezbędne, aby uzyskać obiekt Graph
        // UWAGA: Mimo iż to wywołanie jest redundantne, pozwala uzyskać obiekt Graph 
        // bez modyfikowania struktury klasy Parser z poprzedniego kroku.
        Graph graph = parser.parseGraph(graphFile);


        if (isSSSP) {
            // --- Tryb SSSP (Single-Source Shortest Path) ---
            std::vector<int> sources = parser.parseSSSP(ssFile);
            
            // Czyszczenie pliku wyjsciowego przed rozpoczęciem (dla czasu)
            std::ofstream(ssOutFile, std::ios::trunc).close();
            // Czyszczenie pliku weryfikacyjnego (dla dystansów)
            std::ofstream(ssOutFile + ".dist", std::ios::trunc).close();

            double suma_czasu = 0.0;
            
            // 3. Glowna petla SSSP
            for (int sourceNode : sources) {
                // Licznik przed odpaleniem funkcji
                auto start_time = std::chrono::high_resolution_clock::now();
                
                // Uruchomienie algorytmu Dijkstry
                std::vector<long long> distances = dijkstraSolver.findAllPaths(graph, sourceNode);
                
                // Licznik po funkcji
                auto end_time = std::chrono::high_resolution_clock::now();
                DoubleSeconds elapsed_time = end_time - start_time;
                suma_czasu += elapsed_time.count();
                
                // Zapis szczegółowych dystansów (dla weryfikacji poprawności), tylko jeśli jest plik wyjściowy SSSP
                if (!ssOutFile.empty()) {
                     saveSSSPDistances(ssOutFile + ".dist", sourceNode, distances);
                }
            }

            // Obliczenie średniej czasu
            double average_time = suma_czasu / sources.size();
            
            // 4. Zapis średniego czasu
            // Plik .res zawiera TYLKO czas, aby ułatwić parsowanie przez skrypt testujący
            std::ofstream finalOutFile(ssOutFile); 
            finalOutFile << std::fixed << std::setprecision(6) << "t " << average_time << "\n";
            
        } else if (isP2P) {
            // --- Tryb P2P (Point-to-Point) ---
            std::vector<std::pair<int, int>> queries = parser.parseP2P(p2pFile);
            std::vector<std::pair<int, long long>> results;
            
            double suma_czasu = 0.0;
            
            // 3. Glowna petla P2P
            for (const auto& pair : queries) {
                // Licznik przed odpaleniem funkcji
                auto start_time = std::chrono::high_resolution_clock::now();
                
                // Uruchomienie algorytmu Dijkstry P2P
                std::pair<int, long long> result = dijkstraSolver.findPath(graph, pair.first, pair.second);
                results.push_back(result);
                
                // Licznik po funkcji
                auto end_time = std::chrono::high_resolution_clock::now();
                DoubleSeconds elapsed_time = end_time - start_time;
                suma_czasu += elapsed_time.count();
            }

            // Obliczenie średniej czasu
            double average_time = suma_czasu / queries.size();

            // 4. Zapis czasu i dystansów P2P
            saveP2PResult(p2pOutFile, queries, results, average_time);
        }

    } catch (const std::exception& e) {
        std::cerr << "Blad: Wystapil blad: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Blad: Wystapil nieznany blad.\n";
        return 1;
    }

    return 0;
}