#include "../header/Algorithm.h"
#include <vector>
#include <deque>
#include <unordered_map>
#include "../header/Graph.h"
#include <algorithm>
#include <stdexcept>
#include <string>

std::optional<std::pair<std::vector<int>, std::vector<int>>> Algorithm::isBipartiate(Graph g){
    std::vector<int> set1;
    std::vector<int> set2;
    int numberOfVertices = g.v_size-1;
    int* color = new int[numberOfVertices];
    std::fill(color, color+numberOfVertices, -1);
    std::deque<int> queue;
    //TODO dodaj zeby dzialalo dla izolowanych plus mozna poprawic inicjowanie grafow tak zeby lepiej zczytywal dges z stdina
    
    for(int startnode=0; startnode<numberOfVertices; startnode++){

        if(color[startnode]==-1){
            queue.push_back(startnode);
            color[startnode]=0;
            set1.push_back(startnode);

             while(!queue.empty()){
                int current = queue.front();
                queue.pop_front();

                int left = g.vertices[current];
                int right = g.vertices[current+1];

                for(int i=left; i<right; i++){
                    int neighbour = g.edges[i];

                    if(color[neighbour]==-1){
                        color[neighbour]=1-color[current];
                        queue.push_back(neighbour);

                        if(color[neighbour]==0) set1.push_back(neighbour);
                        else set2.push_back(neighbour);
                    }
                    else if(color[neighbour]==color[current]){
                        delete[] color;
                        return std::nullopt;
                    }
                }
            }
        }
    }

    delete[] color;
    std::pair<std::vector<int>, std::vector<int>> partition = {set1, set2};
    return std::optional<std::pair<std::vector<int>, std::vector<int>>>(partition);
}

std::vector<std::vector<int>> Algorithm::SSC(Graph g){
    std::vector<int> post = sortTopologicaly(g, false);
    Graph reversee = g.reverseGraph();
    int vNumber = g.v_size-1;
    int* visited = new int[vNumber];//-1-unvisited
    std::fill(visited, visited+vNumber, -1);
    std::vector<std::vector<int>> sscs;

    for(int i=post.size()-1; i>=0; i--){
        int current = post[i];
        if(visited[current]==-1){
            std::vector<int> component = search(reversee, AlgType::BFS, post[i], visited, false); //trzeba przekazywac visited najlepiej
            sscs.push_back(component);
        }
    }

    delete[] visited;
    return sscs;
}

class CycleFoundException : public std::runtime_error {
    public:
    CycleFoundException(const std::string& msg) : std::runtime_error(msg) {};
};

std::vector<int> Algorithm::search(Graph g, AlgType t, int start, int* visited, bool treeable){ //visited ma miec -1 jak nie odwiedzony a jak odwiedzony to parenta
    std::vector<int> order;
    int v_size = g.v_size;
    std::deque<int> container;
    container.push_front(start);
    visited[start] = start;

    while(!container.empty()){
        int current = container.front();
        container.pop_front();
        
        int r1 = g.vertices[current];
        int r2 = g.vertices[current+1];
        for(int i=r1; i<r2; i++){
            if(visited[g.edges[i]]==-1){
                if(t==AlgType::BFS) container.push_back(g.edges[i]);
                else container.push_front(g.edges[i]);
                visited[g.edges[i]]=current;
            }
        }
        order.push_back(current);
    }
    return order;
}

std::vector<int> Algorithm::sortTopologicaly(Graph g, bool throwable){
    std::vector<int> order;
    int v_size = g.v_size;
    int* color = new int[v_size-1]{};
    std::deque<int> container;

    for(int startnode=0; startnode<v_size-1; startnode++){
        if(color[startnode]==0){
            container.push_front(startnode);

             while(!container.empty()){
                int current = container.front();
                if(color[current]==0) color[current]=1;
                bool found_new_neighbour = false;
                
                int r1 = g.vertices[current];
                int r2 = g.vertices[current+1];
                for(int i=r1; i<r2; i++){
                    int neighbour = g.edges[i];
                    if(color[neighbour]==0){
                        container.push_front(neighbour);
                        found_new_neighbour=true;
                        break;;
                    } else if (color[g.edges[i]]==1 && throwable){
                        delete[] color;
                        throw CycleFoundException("A cycle has been found");
                    }
                }
                if(found_new_neighbour) continue;
                container.pop_front();
                color[current]=2;
                order.push_back(current); //remember that it must be reserved ;d
            }
        }
    }
    
    delete[] color;
    return order;
}