#ifndef PARSER_H
#define PARSER_H

#include "./Graph.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <limits>

/**
 * @brief Klasa odpowiedzialna za wczytywanie danych z plików DIMACS (.gr, .ss, .p2p).
 */
class Parser {
public:
    // Struktura do przechowywania danych odczytanych z pliku .gr
    struct GraphData {
        int numberOfVertices = 0;
        long long numberOfEdges = 0;
        long long maxCost = 0; // Maksymalny koszt łuku (wymagany dla DIALA i RADIX HEAP jako C)
        Type type = Directed; // Domyślnie Directed, zgodnie z formatem DIMACS
        std::vector<std::tuple<int, int, long long>> edgeList; // (źródło, ujście, waga)
    };

    /**
     * @brief Wczytuje sieć z pliku .gr i tworzy obiekt Graph.
     * @param filename Ścieżka do pliku .gr.
     * @return Obiekt Graph zawierający wczytaną sieć.
     */
    Graph parseGraph(const std::string& filename);

    /**
     * @brief Wczytuje listę źródeł dla problemu SSSP z pliku .ss.
     * @param filename Ścieżka do pliku .ss.
     * @return Wektor indeksów wierzchołków źródłowych (numeracja 1-bazowa).
     */
    std::vector<int> parseSSSP(const std::string& filename);

    /**
     * @brief Wczytuje listę par wierzchołków dla problemu P2P z pliku .p2p.
     * @param filename Ścieżka do pliku .p2p.
     * @return Wektor par (źródło, ujście), numeracja 1-bazowa.
     */
    std::vector<std::pair<int, int>> parseP2P(const std::string& filename);

private:
    /**
     * @brief Wczytuje dane surowe z pliku .gr i zwraca obiekt GraphData.
     * @param filename Ścieżka do pliku .gr.
     * @return Struktura GraphData z listą krawędzi i parametrami grafu.
     */
    GraphData loadGraphData(const std::string& filename);
};

#endif