#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <fstream>
#include <tuple>

using namespace std;

/**
 * Struktura reprezentująca krawędź w sieci przepływowej.
 */
struct Edge {
    int to;
    long long capacity;
    long long flow;
    int rev; // Indeks krawędzi powrotnej w liście sąsiedztwa wierzchołka 'to'
};

/**
 * Funkcja obliczająca wagę Hamminga (liczbę jedynek) dla liczby k-bitowej.
 */
int hammingWeight(int n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1);
        count++;
    }
    return count;
}

/**
 * Funkcja obliczająca liczbę zer w k-bitowej reprezentacji liczby.
 */
int zeroCount(int n, int k) {
    return k - hammingWeight(n);
}

/**
 * Klasa implementująca algorytm Edmondsa-Karpa.
 */
class EdmondsKarp {
    int n;
    vector<vector<Edge>> adj;
    long long totalFlow;
    int augmentingPathsCount;

public:
    EdmondsKarp(int nodes) : n(nodes), adj(nodes), totalFlow(0), augmentingPathsCount(0) {}

    void addEdge(int from, int to, long long capacity) {
        Edge a = {to, capacity, 0, (int)adj[to].size()};
        Edge b = {from, 0, 0, (int)adj[from].size()};
        adj[from].push_back(a);
        adj[to].push_back(b);
    }

    long long solve(int s, int t) {
        while (true) {
            vector<int> parent(n, -1);
            vector<int> edgeIdx(n, -1);
            queue<int> q;

            q.push(s);
            parent[s] = s;

            while (!q.empty() && parent[t] == -1) {
                int v = q.front();
                q.pop();

                for (int i = 0; i < adj[v].size(); ++i) {
                    Edge &e = adj[v][i];
                    if (parent[e.to] == -1 && e.capacity - e.flow > 0) {
                        parent[e.to] = v;
                        edgeIdx[e.to] = i;
                        q.push(e.to);
                    }
                }
            }

            if (parent[t] == -1) break;

            long long push = -1;
            int curr = t;
            while (curr != s) {
                int p = parent[curr];
                int idx = edgeIdx[curr];
                if (push == -1 || adj[p][idx].capacity - adj[p][idx].flow < push) {
                    push = adj[p][idx].capacity - adj[p][idx].flow;
                }
                curr = p;
            }

            totalFlow += push;
            augmentingPathsCount++;
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

    int getAugmentingPathsCount() const { return augmentingPathsCount; }

    void printFlows(int k) {
        for (int i = 0; i < n; ++i) {
            for (const auto &e : adj[i]) {
                if (e.capacity > 0) {
                    cout << i << " -> " << e.to << " [przeplyw: " << e.flow << "/" << e.capacity << "]" << endl;
                }
            }
        }
    }
};

/**
 * Funkcja generująca model GLPK MathProg dla problemu maksymalnego przepływu.
 */
void generateGLPKModel(const string& filename, int n, int s, int t, const vector<tuple<int, int, long long>>& edges) {
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "Nie mozna otworzyc pliku do zapisu modelu GLPK." << endl;
        return;
    }

    ofs << "/* Model Programowania Liniowego dla problemu maksymalnego przeplywu */\n\n";

    // Zbiory
    ofs << "set V := 0.." << (n - 1) << ";\n";
    // Definiujemy E jako zbiór par zapisanych w nawiasach klamrowych — składnia akceptowana przez GLPK
    ofs << "set E := {\n";
    for (size_t i = 0; i < edges.size(); ++i) {
        auto [u, v, cap] = edges[i];
        ofs << "  (" << u << "," << v << ")" << (i == edges.size() - 1 ? "\n" : ",\n");
    }
    ofs << "};\n\n";

    // Parametry (pojemności)
    ofs << "param cap{ (u,v) in E };\n\n";
    
    // Dane pojemności
    ofs << "/* Definicja pojemnosci dla kazdego luku */\n";
    ofs << "data;\n";
    ofs << "param cap :=\n";
    for (const auto& [u, v, cap] : edges) {
        ofs << "  " << u << " " << v << " " << cap << "\n";
    }
    ofs << ";\n";
    ofs << "end;\n\n";

    // Zmienne decyzyjne
    ofs << "/* x[u,v] oznacza wielkosc przeplywu przez luk (u,v) */\n";
    ofs << "var x{ (u,v) in E } >= 0, <= cap[u,v];\n\n";

    // Funkcja celu
    ofs << "/* Maksymalizujemy wyplyw ze zrodla */\n";
    ofs << "maximize MaxFlow: sum{ (s,v) in E: s = " << s << " } x[s,v] - sum{ (v,s) in E: s = " << s << " } x[v,s];\n\n";

    // Ograniczenia zachowania przepływu
    ofs << "/* Prawo zachowania przeplywu dla wezlow posrednich */\n";
    ofs << "s.t. conservation{ i in V: i != " << s << " and i != " << t << " }:\n";
    ofs << "  sum{ (j,i) in E } x[j,i] - sum{ (i,j) in E } x[i,j] = 0;\n\n";

    ofs << "solve;\n";
    ofs << "display MaxFlow;\n";
    ofs << "/* display x; */\n";
    ofs << "end;\n";

    ofs.close();
    cerr << "Model GLPK zapisany w pliku: " << filename << endl;
}


int main(int argc, char* argv[]) {
    int k = -1;
    bool printFlow = false;
    string glpkFile = "";

    // Parsowanie parametrów linii komend
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--size" && i + 1 < argc) {
            k = stoi(argv[++i]);
        } else if (arg == "--printFlow") {
            printFlow = true;
        } else if (arg == "--glpk" && i + 1 < argc) {
            glpkFile = argv[++i];
        }
    }

    if (k < 1 || k > 16) {
        cerr << "Blad: Parametr --size k musi byc w zakresie [1, 16]." << endl;
        return 1;
    }

    int numNodes = 1 << k;
    EdmondsKarp ek(numNodes);

    // Generator liczb losowych
    random_device rd;
    mt19937 gen(rd());

    // Przechowywanie krawędzi do wygenerowania modelu LP
    vector<tuple<int, int, long long>> allEdges;

    // Budowa grafu Hiperkostki Hk
    for (int i = 0; i < numNodes; ++i) {
        for (int bit = 0; bit < k; ++bit) {
            int j = i | (1 << bit);
            // Krawędź istnieje jeśli bity różnią się o 1 i waga Hamminga rośnie
            if (i != j) {
                int hi = hammingWeight(i);
                int hj = hammingWeight(j);
                
                // Z definicji: krawędź prowadzi od mniejszej wagi do większej
                if (hj == hi + 1) {
                    int zi = zeroCount(i, k);
                    int zj = zeroCount(j, k);
                    
                    int l = max({hi, zi, hj, zj});
                    long long max_cap = 1LL << l;
                    
                    uniform_int_distribution<long long> dist(1, max_cap);
                    long long capacity = dist(gen);
                    
                    ek.addEdge(i, j, capacity);
                    
                    if (!glpkFile.empty()) {
                        allEdges.emplace_back(i, j, capacity);
                    }
                }
            }
        }
    }

    // Generowanie modelu GLPK przed uruchomieniem algorytmu (lub po, bez znaczenia dla wyniku)
    if (!glpkFile.empty()) {
        generateGLPKModel(glpkFile, numNodes, 0, numNodes - 1, allEdges);
    }

    auto start = chrono::high_resolution_clock::now();
    
    long long maxFlowValue = ek.solve(0, numNodes - 1);
    
    auto end = chrono::high_resolution_clock::now();
    // Mierzymy czas w mikrosekundach, żeby mieć większą rozdzielczość dla szybkich uruchomień
    long long elapsed_us = chrono::duration_cast<chrono::microseconds>(end - start).count();

    // Wynik na stdout
    cout << "Maksymalny przeplyw: " << maxFlowValue << endl;
    if (printFlow) {
        ek.printFlows(k);
    }

    // Statystyki na stderr
    cerr << "Czas wykonania: " << elapsed_us << " us" << endl;
    cerr << "Liczba sciezek powiekszajacych: " << ek.getAugmentingPathsCount() << endl;

    return 0;
}