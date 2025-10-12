#ifndef GRAPH_H
#define GRAPH_H

#include "./Type.h"
#include <vector>
#include <utility>

class Graph {
    public:
    int* vertices;
    int* edges;
    int v_size;
    int e_size;
    Type type;
    Graph(int numberOfVertices, char t, std::vector<std::pair<int,int>> edges);
    ~Graph();
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    Graph(Graph&& other) noexcept;
    Graph& operator=(Graph&& other) noexcept;
    private:
    void setType(char t);
    void setArrays(int n, std::vector<std::pair<int,int>> edges);
};

#endif