#ifndef GRAPH_H
#define GRAPH_H

#include "./Type.h"
#include <vector>
#include <utility>

class Graph {
    public:
    int* vertices;
    int* edges;
    int* weights;
    int max_weight;
    int min_weight;
    int v_size;
    int e_size;
    int w_size;
    Type type;
    Graph(){};
    Graph(int* vertices, int* edges, int* weights, int vsize, int esize, int wsize) : vertices(vertices), edges(edges), weights(weights),v_size(vsize), e_size(esize), w_size(wsize) {};
    Graph(int numberOfVertices, char t, std::vector<std::pair<int ,std::pair<int,int>>> edges);
    ~Graph();
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    Graph(Graph&& other) noexcept;
    Graph& operator=(Graph&& other) noexcept;
    Graph reverseGraph();
    std::vector<std::pair<int, int>> getNeighbours(int current) const;
    private:
    void setType(char t);
    void setArrays(int n, std::vector<std::pair<int ,std::pair<int,int>>> edges);
};

#endif