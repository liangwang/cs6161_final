#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "graph.hh"


int main(int argc, char** argv)
{
    Graph g;
    g.Build();
    int node_count = g.VertexCounts();

    srand((unsigned)time(0));
    int s = rand() % node_count;

    std::vector<weight_t> dist;
    std::vector<vertex_t> previous;
    g.DijkstraShortestPath(s, dist, previous);

    std::cout<<">>>>>>>>>>>>>> Fibonacci Heap" << std::endl;
    // std::cout<<"The shortest path from node " << s <<" are:" << std::endl;

    // for (int i = 0; i < node_count; i++)
    // {
    //     std::cout<<"to node " << i << ": " << dist[i] << "\t" << previous[i] << std::endl;
    // }

    g.SetHeapPolicy(PriorityQueue);

    std::vector<weight_t> dist2;
    std::vector<vertex_t> previous2;
    g.DijkstraShortestPath(s, dist2, previous2);

    std::cout<<">>>>>>>>>>> Priority Queue" << std::endl;

    for (int i = 0; i < node_count; i ++) {
        if (previous[i] != previous2[i]) {
            std::cout<<"Diff" << std::endl;
            break;
        }
    }
    // for (int i = 0; i < node_count; i++)
    // {
    //     std::cout<<"to node " << i << ": " << dist[i] << "\t" << previous[i] << std::endl;
    // }


    return EXIT_SUCCESS;
}
