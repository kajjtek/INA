#include "../include/Parser.h"

// Funkcja pomocnicza do przetwarzania linii i ignorowania komentarzy 'c'
std::string getNextDataLine(std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        // Usuń białe znaki na początku i na końcu
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);
        
        // Zignoruj komentarze
        if (line.empty() || line[0] == 'c') {
            continue;
        }
        return line;
    }
    return ""; // Pusty ciąg, jeśli EOF
}

/**
 * @brief Wczytuje sieć z pliku .gr i tworzy obiekt Graph.
 * @param filename Ścieżka do pliku .gr.
 * @return Obiekt Graph zawierający wczytaną sieć.
 */
Graph Parser::parseGraph(const std::string& filename) {
    GraphData data = loadGraphData(filename);

    // Konwersja listy krawędzi na format wejściowy dla konstruktora Graph
    // Zakładam, że konstruktor Graph(int n, char t, std::vector<std::pair<int, std::pair<int, int>>> edges)
    // oczekuje listy w formacie: (waga, (źródło, ujście)).
    // UWAGA: Trzeba dostosować do faktycznej implementacji konstruktora w Graph.h
    std::vector<std::pair<long long, std::pair<int, int>>> graphEdges;
    
    for (const auto& edge : data.edgeList) {
        int u = std::get<0>(edge);
        int v = std::get<1>(edge);
        long long w = std::get<2>(edge);
        
        // W formacie DIMACS: a u v w
        graphEdges.push_back({w, {u, v}});
    }

    // Wymagamy maksymalnego kosztu łuku C, aby przekazać go do DIALA/RADIX HEAP.
    // W obecnej strukturze Graph.h nie ma miejsca na C, ale możemy to później przekazać.

    // Tworzenie obiektu Graph.
    // Zakładam, że t='d' (directed) dla ogólnego problemu SP
    return Graph(data.numberOfVertices, 'd', graphEdges);
}

/**
 * @brief Wczytuje surowe dane z pliku .gr (nagłówek i listę krawędzi)
 */
Parser::GraphData Parser::loadGraphData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku grafu: " << filename << std::endl;
        throw std::runtime_error("Nie mozna otworzyc pliku grafu.");
    }

    GraphData data;
    data.maxCost = 0;
    
    std::string line;
    while (!(line = getNextDataLine(file)).empty()) {
        std::stringstream ss(line);
        char type;
        
        ss >> type;

        if (type == 'p') {
            // Nagłówek: p sp N M (N - wierzchołki, M - łuki)
            std::string problemType;
            ss >> problemType >> data.numberOfVertices >> data.numberOfEdges;
            // Sprawdzenie, czy to na pewno Shortest Path (sp)
            if (problemType != "sp") {
                std::cerr << "Ostrzezenie: Niepoprawny typ problemu w naglowku .gr. Oczekiwano 'sp'." << std::endl;
            }
        } else if (type == 'a') {
            // Łuk: a u v w (u - źródło, v - ujście, w - waga/koszt)
            int u, v;
            long long w;
            ss >> u >> v >> w;
            
            // Wymóg zadania: koszty są nieujemne (dla Dijkstry).
            if (w < 0) {
                 std::cerr << "Blad: Waga ujemna znaleziona. Dijkstra nie bedzie dzialac poprawnie. w = " << w << std::endl;
                 throw std::runtime_error("Waga ujemna w grafie.");
            }

            data.edgeList.emplace_back(u, v, w);
            data.maxCost = std::max(data.maxCost, w);

        } else {
            std::cerr << "Ostrzezenie: Nieznana linia w pliku .gr: " << line << std::endl;
        }
    }
    
    // Upewniamy się, że wczytaliśmy wszystkie łuki
    if (data.edgeList.size() != data.numberOfEdges) {
        std::cerr << "Ostrzezenie: Niezgodnosc liczby lukow. Oczekiwano: " << data.numberOfEdges << ", wczytano: " << data.edgeList.size() << std::endl;
    }
    
    std::cout << "Wczytano graf. Wierzcholki: " << data.numberOfVertices 
              << ", Lukow: " << data.edgeList.size() 
              << ", Max koszt C: " << data.maxCost << std::endl;

    return data;
}

/**
 * @brief Wczytuje listę źródeł dla problemu SSSP z pliku .ss.
 * @param filename Ścieżka do pliku .ss.
 * @return Wektor indeksów wierzchołków źródłowych (numeracja 1-bazowa).
 */
std::vector<int> Parser::parseSSSP(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku zrodla.ss: " << filename << std::endl;
        throw std::runtime_error("Nie mozna otworzyc pliku zrodla.ss.");
    }

    std::vector<int> sources;
    std::string line;
    int expectedSources = 0;

    while (!(line = getNextDataLine(file)).empty()) {
        std::stringstream ss(line);
        char type;
        
        ss >> type;

        if (type == 'p') {
            // Nagłówek: p aux sp ss K (K - liczba źródeł)
            std::string aux, problem, problemType;
            ss >> aux >> problem >> problemType >> expectedSources;
            if (problemType != "ss") {
                 std::cerr << "Ostrzezenie: Niepoprawny typ problemu w naglowku .ss. Oczekiwano 'ss'." << std::endl;
            }
        } else if (type == 's') {
            // Źródło: s V (V - indeks wierzchołka)
            int sourceNode;
            ss >> sourceNode;
            sources.push_back(sourceNode-1);
        } else {
            // Ignorujemy inne, jeśli istnieją, ale w tym formacie nie powinny.
        }
    }
    
    if (sources.size() != expectedSources) {
        std::cerr << "Ostrzezenie: Niezgodnosc liczby zrodel. Oczekiwano: " << expectedSources << ", wczytano: " << sources.size() << std::endl;
    }

    return sources;
}

/**
 * @brief Wczytuje listę par wierzchołków dla problemu P2P z pliku .p2p.
 * @param filename Ścieżka do pliku .p2p.
 * @return Wektor par (źródło, ujście), numeracja 1-bazowa.
 */
std::vector<std::pair<int, int>> Parser::parseP2P(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku pary.p2p: " << filename << std::endl;
        throw std::runtime_error("Nie mozna otworzyc pliku pary.p2p.");
    }

    std::vector<std::pair<int, int>> pairs;
    std::string line;
    int expectedPairs = 0;

    while (!(line = getNextDataLine(file)).empty()) {
        std::stringstream ss(line);
        char type;
        
        ss >> type;

        if (type == 'p') {
            // Nagłówek: p aux sp p2p K (K - liczba par)
            std::string aux, problem, problemType;
            ss >> aux >> problem >> problemType >> expectedPairs;
            if (problemType != "p2p") {
                 std::cerr << "Ostrzezenie: Niepoprawny typ problemu w naglowku .p2p. Oczekiwano 'p2p'." << std::endl;
            }
        } else if (type == 'q') {
            // Para: q U V (U - źródło, V - ujście)
            int source, target;
            ss >> source >> target;
            // store as 0-based indices
            pairs.push_back({source - 1, target - 1});
        } else {
            // Ignorujemy
        }
    }
    
    if (pairs.size() != expectedPairs) {
        std::cerr << "Ostrzezenie: Niezgodnosc liczby par. Oczekiwano: " << expectedPairs << ", wczytano: " << pairs.size() << std::endl;
    }

    return pairs;
}