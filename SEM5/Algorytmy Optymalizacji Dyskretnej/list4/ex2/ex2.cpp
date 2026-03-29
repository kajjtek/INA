#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include <set>
#include <fstream>

using namespace std;

/**
 * Struktura reprezentująca krawędź w sieci przepływowej.
 */
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev; // Indeks krawędzi powrotnej
};

/**
 * Klasa implementująca algorytm Edmondsa-Karpa do obliczania Max Flow.
 */
class EdmondsKarp {
    int n;
    vector<vector<Edge>> adj;

public:
    EdmondsKarp(int nodes) : n(nodes), adj(nodes) {}

    void addEdge(int from, int to, int capacity) {
        Edge a = {to, capacity, 0, (int)adj[to].size()};
        Edge b = {from, 0, 0, (int)adj[from].size()};
        adj[from].push_back(a);
        adj[to].push_back(b);
    }

    int solve(int s, int t) {
        int totalFlow = 0;
        while (true) {
            vector<int> parent(n, -1);
            vector<int> edgeIdx(n, -1);
            queue<int> q;

            q.push(s);
            parent[s] = s;

            while (!q.empty() && parent[t] == -1) {
                int v = q.front();
                q.pop();

                for (int i = 0; i < (int)adj[v].size(); ++i) {
                    Edge &e = adj[v][i];
                    if (parent[e.to] == -1 && e.capacity - e.flow > 0) {
                        parent[e.to] = v;
                        edgeIdx[e.to] = i;
                        q.push(e.to);
                    }
                }
            }

            if (parent[t] == -1) break;

            int push = 1e9;
            int curr = t;
            while (curr != s) {
                int p = parent[curr];
                int idx = edgeIdx[curr];
                push = min(push, adj[p][idx].capacity - adj[p][idx].flow);
                curr = p;
            }

            totalFlow += push;
            curr = t;
            while (curr != s) {
                int p = parent[curr];
                int idx = edgeIdx[curr];
                int revIdx = adj[p][idx].rev;
                adj[p][idx].flow += push;
                adj[curr][revIdx].flow -= push;
                curr = p;
            }
        }
        return totalFlow;
    }

    vector<pair<int, int>> getMatchingEdges(int sizeV1) {
        vector<pair<int, int>> matching;
        for (int u = 1; u <= sizeV1; ++u) {
            for (const auto &e : adj[u]) {
                if (e.to > sizeV1 && e.to <= 2 * sizeV1 && e.flow == 1) {
                    matching.push_back({u - 1, e.to - sizeV1 - 1});
                }
            }
        }
        return matching;
    }
};

/**
 * Funkcja generująca model GLPK MathProg dla problemu maksymalnego skojarzenia.
 */
void generateGLPKModel(const string& filename, int sizeV, const vector<pair<int, int>>& allEdges) {
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "Nie mozna otworzyc pliku do zapisu modelu GLPK." << endl;
        return;
    }

    ofs << "/* Model Programowania Liniowego dla problemu maksymalnego skojarzenia */\n\n";
    
    // Zbiory wierzchołków
    ofs << "set V1 := 0.." << (sizeV - 1) << ";\n";
    ofs << "set V2 := 0.." << (sizeV - 1) << ";\n\n";

    // Zbiór krawędzi
        ofs << "set E := {\n";
    for (size_t i = 0; i < allEdges.size(); ++i) {
            ofs << "  (" << allEdges[i].first << ", " << allEdges[i].second << ")" << (i == allEdges.size() - 1 ? "\n" : ",\n");
    }
        ofs << "};\n\n";

    // Zmienne decyzyjne - binarna zmienna dla każdej krawędzi
    ofs << "/* x[u,v] = 1 jesli krawedz nalezy do skojarzenia, 0 w p.p. */\n";
    ofs << "var x{ (u,v) in E } binary;\n\n";

    // Funkcja celu
    ofs << "/* Maksymalizujemy liczbe wybranych krawedzi */\n";
    ofs << "maximize TotalMatchingSize: sum{ (u,v) in E } x[u,v];\n\n";

    // Ograniczenia
    ofs << "/* Kazdy wierzcholek z V1 moze byc czescia najwyzej jednej krawedzi */\n";
    ofs << "s.t. limitV1{u in V1}: sum{ v in V2: (u,v) in E } x[u,v] <= 1;\n\n";

    ofs << "/* Kazdy wierzcholek z V2 moze byc czescia najwyzej jednej krawedzi */\n";
    ofs << "s.t. limitV2{v in V2}: sum{ u in V1: (u,v) in E } x[u,v] <= 1;\n\n";

    ofs << "solve;\n";
    ofs << "display TotalMatchingSize;\n";
    ofs << "/* display x; */\n";
    ofs << "end;\n";

    ofs.close();
    cerr << "Model GLPK zapisany w pliku: " << filename << endl;
}

int main(int argc, char* argv[]) {
    int k = -1;
    int degreeI = -1;
    bool printMatching = false;
    string glpkFile = "";

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--size" && i + 1 < argc) {
            k = stoi(argv[++i]);
        } else if (arg == "--degree" && i + 1 < argc) {
            degreeI = stoi(argv[++i]);
        } else if (arg == "--printMatching") {
            printMatching = true;
        } else if (arg == "--glpk" && i + 1 < argc) {
            glpkFile = argv[++i];
        }
    }

    if (k < 1 || k > 16 || degreeI < 0) {
        cerr << "Blad: Nieprawidlowe parametry. Uzycie: --size k --degree i [--printMatching] [--glpk nazwa]" << endl;
        return 1;
    }

    int sizeV = 1 << k;
    int source = 0;
    int sink = 2 * sizeV + 1;
    EdmondsKarp ek(sink + 1);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, sizeV - 1);

    vector<pair<int, int>> allEdges; // Przechowujemy krawędzie do modelu LP

    for (int u = 0; u < sizeV; ++u) {
        ek.addEdge(source, u + 1, 1);
        ek.addEdge(u + sizeV + 1, sink, 1);

        set<int> neighbors;
        while ((int)neighbors.size() < degreeI) {
            neighbors.insert(dist(gen));
        }

        for (int v : neighbors) {
            ek.addEdge(u + 1, v + sizeV + 1, 1);
            allEdges.push_back({u, v});
        }
    }

    if (!glpkFile.empty()) {
        generateGLPKModel(glpkFile, sizeV, allEdges);
    }

    auto startTime = chrono::high_resolution_clock::now();
    int maxMatchingSize = ek.solve(source, sink);
    auto endTime = chrono::high_resolution_clock::now();
    // Mierzymy czas w mikrosekundach
    long long elapsed_us = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << "Rozmiar maksymalnego skojarzenia: " << maxMatchingSize << endl;
    if (printMatching) {
        cout << "Krawedzie skojarzenia (V1 -> V2):" << endl;
        auto matching = ek.getMatchingEdges(sizeV);
        for (auto const& edge : matching) {
            cout << edge.first << " - " << edge.second << endl;
        }
    }

    cerr << "Czas wykonania (Edmonds-Karp): " << elapsed_us << " us" << endl;

    return 0;
}