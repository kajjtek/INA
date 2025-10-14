#include "../header/Algorithm.h"
#include <vector>
#include <deque>
#include <unordered_map>
#include "../header/Graph.h"
#include <algorithm>
#include <exception>

class CycleFoundException : public std::runtime_error {
    public:
    CycleFoundException(const std::string& msg) : std::runtime_error(msg) {};
};

std::vector<int> Algorithm::search(Graph g, AlgType t, int start){
    std::vector<int> order;
    int v_size = g.v_size;
    bool* visited = new bool[v_size-1];
    std::fill(visited, visited+v_size-1, false);
    std::deque<int> container;
    container.push_front(start);
    visited[start] = true;

    while(!container.empty()){
        int current = container.front();
        container.pop_front();
        
        int r1 = g.vertices[current];
        int r2 = g.vertices[current+1];
        for(int i=r1; i<r2; i++){
            if(!visited[g.edges[i]]){
                if(t==AlgType::BFS) container.push_back(g.edges[i]);
                else container.push_front(g.edges[i]);
                visited[g.edges[i]]=true;
            } else {
                throw CycleFoundException("A cycle has been found");
            }
        }
        order.push_back(current+1);
    }

    delete[] visited;
    return order;
}

std::vector<int> Algorithm::sortTopologicaly(Graph g){
    std::vector<int> order;
    int v_size = g.v_size;
    int* color = new int[v_size-1]{};
    std::deque<int> container;
    container.push_front(0);
    

    while(!container.empty()){
        int current = container.front();
        color[current] = 1;
        bool should_continue = true;
        
        int r1 = g.vertices[current];
        int r2 = g.vertices[current+1];
        for(int i=r1; i<r2; i++){
            if(color[g.edges[i]]==0){
                container.push_front(g.edges[i]);
                should_continue=false;
                continue;
            } else if (color[g.edges[i]]==1){
                throw CycleFoundException("A cycle has been found");
            }
        }
        if(!should_continue) continue;
        container.pop_front();
        color[current]=2;
        order.push_back(current+1); //remember that it must be reserved ;d
    }

    delete[] color;
    return order;
}