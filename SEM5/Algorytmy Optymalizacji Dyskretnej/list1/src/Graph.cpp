#include "../header/Graph.h"
#include "../header/Type.h"
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

Graph Graph::reverseGraph(){
    int* nCounts = new int[v_size]{};
    int* nEdges = new int[e_size];
    int* position = new int[v_size-1]{};

    for(int i=0; i<v_size; i++){
        int r1 = vertices[i];
        int r2 = vertices[i+1];

        for(int j=r1; j<r2; j++){
            int v = edges[j];
            nCounts[v]++;
        }
    }

    int* nVertices = new int[v_size];
    nVertices[0]=0;
    for(int i=1; i<v_size; i++){
        nVertices[i]=nVertices[i-1]+nCounts[i];
    }

    for(int i=0; i<v_size; i++){
        int r1 = vertices[i];
        int r2 = vertices[i+1];

        for(int j=r1; j<r2; j++){
            int v = edges[j];
            int u = i;
            int left = nVertices[v];
            int shift = position[v];
            position[v]++;
            nEdges[left+shift]=u;
        }
    }

    Graph nGraph(nVertices, nEdges, v_size, e_size);

    return nGraph;
}

Graph::Graph(int numberOfVertices, char t, std::vector<std::pair<int,int>> edges){
    try {
        setType(t);
        setArrays(numberOfVertices, edges);
        this->v_size=numberOfVertices+1;
        this->e_size=edges.size();
    } catch (const std::invalid_argument& e){
        std::cerr <<"Type class Error !" <<std::endl;
        std::cerr<<"Error:"<<e.what()<<std::endl;
    } catch (const std::exception& e) {
        std::cerr <<"Standard Error !" <<std::endl;
        std::cerr<<"Error:"<<e.what()<<std::endl;
    } catch (...) {
        std::cerr <<"Undefined Error !" <<std::endl;
    }
}

void Graph::setType(char t){
    if(t=='D') this->type=Type::Directed;
    else if (t=='A') this->type=Type::Undirected;
    else throw std::invalid_argument("The type input must be either A or D");
}

void Graph::setArrays(int n, std::vector<std::pair<int,int>> edgesImported){//to dziala dla posortowanych edgesImproted TODO dodac to w pliku ktory przerabia stdina
    int* counts = new int[n+1]{};//to inicjalizuje na zero zapameitaj sobie
    this->vertices=new int[n+1];
    this->edges=new int[edgesImported.size()];
    for(int i=0; i<edgesImported.size();i++){
        std::pair<int,int> Pair = edgesImported.at(i);
        int u = Pair.first-1;
        int v = Pair.second-1;

        counts[u]++;
        this->edges[i]=v;//pamietaj ze u na koncu musi miec dodane +1 bo 0-index
    }
    this->vertices[0]=0;
    for(int i=1; i<n+1;i++){
        this->vertices[i]=this->vertices[i-1]+counts[i-1];
    }
}

Graph::~Graph(){
    delete[] this->vertices;
    delete[] this->edges;
    this->vertices=nullptr;
    this->edges=nullptr;
}

Graph::Graph(const Graph& other) : v_size(other.v_size), e_size(other.e_size) {
    this->vertices=new int[v_size];
    this->edges=new int[e_size];

    std::copy(other.vertices, other.vertices+other.v_size, this->vertices);
    std::copy(other.edges, other.edges+other.e_size, this->edges);
}

Graph& Graph::operator=(const Graph& other){
    if(this==&other){
        return *this;
    }

    delete[] this->vertices;
    delete[] this->edges;

    this->v_size=other.v_size;
    this->e_size=other.e_size;

    this->vertices=new int[v_size];
    this->edges=new int[e_size];

    std::copy(other.vertices, other.vertices+other.v_size, this->vertices);
    std::copy(other.edges, other.edges+other.e_size, this->edges);
    return *this;
}

Graph::Graph(Graph&& other) noexcept : v_size(other.v_size), e_size(other.e_size) {
    other.vertices=nullptr;
    other.edges=nullptr;
    other.v_size=0;
    other.e_size=0;
}

Graph& Graph::operator=(Graph&& other) noexcept{
    if(this==&other){
        return *this;
    }

    delete[] this->vertices;
    delete[] this->edges;

    this->v_size=other.v_size;
    this->e_size=other.e_size;
    this->vertices=other.vertices;
    this->edges=other.edges;
    
    other.vertices=nullptr;
    other.edges=nullptr;
    other.v_size=0;
    other.e_size=0;
    
    return *this;
}