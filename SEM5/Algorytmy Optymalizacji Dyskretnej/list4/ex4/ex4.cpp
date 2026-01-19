#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <tuple>
#include <fstream>
#include <iomanip>

using namespace std;

const long long INF = 1e18;

/**
 * Struktura reprezentująca krawędź.
 */
struct Edge {
    int to;
    long long capacity;
    long long flow;
    int rev; // Indeks krawędzi powrotnej
};

/**
 * Klasa implementująca algorytm Dynica.
 * Złożoność: O(V^2 * E)
 */
class Dinic {
    int n;
    vector<vector<Edge>> adj;
    vector<int> level; // Poziomy wierzchołków w grafie warstwowym
    vector<int> ptr;   // Wskaźnik do "aktualnej" krawędzi (optymalizacja)
    long long totalFlow;
    int augmentingPathsCount;

public:
    Dinic(int nodes) : n(nodes), adj(nodes), level(nodes), ptr(nodes), totalFlow(0), augmentingPathsCount(0) {}

    void addEdge(int from, int to, long long capacity) {
        Edge a = {to, capacity, 0, (int)adj[to].size()};
        Edge b = {from, 0, 0, (int)adj[from].size()};
        adj[from].push_back(a);
        adj[to].push_back(b);
    }

    // BFS budujący graf warstwowy (Level Graph)
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (const auto& e : adj[v]) {
                if (e.capacity - e.flow > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    // DFS szukający przepływu blokującego w grafie warstwowym
    long long dfs(int v, int t, long long pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;

        // Przeglądamy krawędzie od ostatnio zapamiętanej pozycji (ptr[v])
        for (int& cid = ptr[v]; cid < adj[v].size(); ++cid) {
            auto& e = adj[v][cid];
            int tr = e.to;

            if (level[v] + 1 != level[tr] || e.capacity - e.flow == 0) continue;

            long long tr_pushed = dfs(tr, t, min(pushed, e.capacity - e.flow));
            if (tr_pushed == 0) continue;

            e.flow += tr_pushed;
            adj[tr][e.rev].flow -= tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    long long solve(int s, int t) {
        totalFlow = 0;
        augmentingPathsCount = 0;

        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            
            while (long long pushed = dfs(s, t, INF)) {
                totalFlow += pushed;
                augmentingPathsCount++;
            }
        }
        return totalFlow;
    }

    int getAugmentingPathsCount() const { return augmentingPathsCount; }

    void printFlows() {
        for (int i = 0; i < n; ++i) {
            for (const auto &e : adj[i]) {
                if (e.capacity > 0) { // Wyświetlamy tylko oryginalne krawędzie
                     cout << i << " -> " << e.to << " [przeplyw: " << e.flow << "/" << e.capacity << "]" << endl;
                }
            }
        }
    }
    
    // Pobiera wszystkie krawędzie do eksportu (np. GLPK)
    vector<tuple<int, int, long long>> getAllEdges() {
        vector<tuple<int, int, long long>> edges;
        for(int u=0; u<n; ++u) {
            for(const auto& e : adj[u]) {
                if(e.capacity > 0) { // Tylko krawędzie "w przód" z oryginalnego grafu
                    edges.emplace_back(u, e.to, e.capacity);
                }
            }
        }
        return edges;
    }
};

// --- Funkcje pomocnicze (Hamming, Zera) ---

int hammingWeight(int n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1);
        count++;
    }
    return count;
}

int zeroCount(int n, int k) {
    return k - hammingWeight(n);
}

// --- Generowanie modelu GLPK ---

void generateGLPKModel(const string& filename, int n, int s, int t, const vector<tuple<int, int, long long>>& edges) {
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "Nie mozna otworzyc pliku do zapisu modelu GLPK." << endl;
        return;
    }

    ofs << "/* Model Programowania Liniowego dla problemu maksymalnego przeplywu (Hiperkostka) */\n\n";

    ofs << "set V := 0.." << (n - 1) << ";\n";
    ofs << "set E within {V, V} :=\n";
    for (size_t i = 0; i < edges.size(); ++i) {
        auto [u, v, cap] = edges[i];
        ofs << "  (" << u << "," << v << ")" << (i == edges.size() - 1 ? ";" : ",") << "\n";
    }
    ofs << "\n";

    ofs << "param cap{ (u,v) in E };\n\n";
    
    ofs << "data;\n";
    ofs << "param cap :=\n";
    for (const auto& [u, v, cap] : edges) {
        ofs << "  " << u << " " << v << " " << cap << "\n";
    }
    ofs << ";\n";
    ofs << "end;\n\n";

    ofs << "var x{ (u,v) in E } >= 0, <= cap[u,v];\n\n";

    ofs << "maximize MaxFlow: sum{ (s,v) in E: s = " << s << " } x[s,v] - sum{ (v,s) in E: s = " << s << " } x[v,s];\n\n";

    ofs << "s.t. conservation{ i in V: i != " << s << " and i != " << t << " }:\n";
    ofs << "  sum{ (j,i) in E } x[j,i] - sum{ (i,j) in E } x[i,j] = 0;\n\n";

    ofs << "solve;\n";
    ofs << "display MaxFlow;\n";
    ofs << "end;\n";

    ofs.close();
    cerr << "Model GLPK zapisany w pliku: " << filename << endl;
}

// --- Main ---

int main(int argc, char* argv[]) {
    int k = -1;
    bool printFlow = false;
    string glpkFile = "";

    // Parsowanie parametrów
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
    Dinic dinic(numNodes);

    random_device rd;
    mt19937 gen(rd());

    // Budowa Hiperkostki Hk (identyczna jak w Zadaniu 1)
    for (int i = 0; i < numNodes; ++i) {
        for (int bit = 0; bit < k; ++bit) {
            int j = i | (1 << bit);
            
            if (i != j) {
                int hi = hammingWeight(i);
                int hj = hammingWeight(j);
                
                // Krawędź prowadzi od mniejszej wagi Hamminga do większej
                if (hj == hi + 1) {
                    int zi = zeroCount(i, k);
                    int zj = zeroCount(j, k);
                    
                    int l = max({hi, zi, hj, zj});
                    long long max_cap = 1LL << l;
                    
                    uniform_int_distribution<long long> dist(1, max_cap);
                    long long capacity = dist(gen);
                    
                    dinic.addEdge(i, j, capacity);
                }
            }
        }
    }

    // Opcjonalne generowanie modelu GLPK
    if (!glpkFile.empty()) {
        generateGLPKModel(glpkFile, numNodes, 0, numNodes - 1, dinic.getAllEdges());
    }

    // Pomiar czasu
    auto start = chrono::high_resolution_clock::now();
    
    long long maxFlowValue = dinic.solve(0, numNodes - 1);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Wyjście
    cout << "Maksymalny przeplyw (Dinic): " << maxFlowValue << endl;
    if (printFlow) {
        dinic.printFlows();
    }

    // Statystyki na stderr
    cerr << "Czas wykonania: " << elapsed.count() << " s" << endl;
    cerr << "Liczba sciezek powiekszajacych (DFS hits): " << dinic.getAugmentingPathsCount() << endl;

    return 0;
}