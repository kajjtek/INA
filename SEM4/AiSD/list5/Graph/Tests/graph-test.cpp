#include "../Headers/Graph.h"
#include "../Headers/GreedyAlgorithms.h"
#include <iostream>

int main(){
    Graph graph;
    graph.createRandomFullGraph(5);
    GreedyAlgorithms util;
    Graph mst = util.prism(graph);
    Graph mst2 = util.Kruskal(graph);
}