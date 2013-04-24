#ifndef _GRAPH_H
#define _GRAPH_H

#include <string>
#include <vector>
#include <limits>
#include <utility>

typedef int vertex_t;
typedef float weight_t;

const weight_t INF_WEIGHT = std::numeric_limits<weight_t>::infinity();
const vertex_t VERTEX_UNDEFINED = -1;

enum HeapType {
    Fibonacci,
    Binomial,
    Binary,
    PriorityQueue,
    NumHeaps
};

typedef struct neighbor {
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
}neighbor_t;

typedef std::vector<std::vector<neighbor_t> > adjacency_list_t;


class Graph {
public:
    /*
     * Default constructor
     *   Initialize class members
     */
    Graph();

    /*
     * Build graph from a given file
     */
    void Build(const char *filename);
    void Build(const std::string &filename);
    
    /*
     * Build a sample graph, just for test
     */
    inline void Build() {Build("sample.graph");};

    void AddEdge(const vertex_t v1, const vertex_t v2, const weight_t w);


    /*
     * Get a list of all neighbors from a vertex
     */
    const std::vector<neighbor_t>& Neighbors(const vertex_t v) const;

    /*
     * Test whether there is a edge between x and y
     *   not sure needed or not.
     */
    bool Adjacent(const vertex_t x, const vertex_t y) const;

    /*
     * Shortest path from a single source s, using Dijkstra algorithm
     * Args:
     *  - s, the source node
     *  - dist, the output array for the distances of the shortest path from s
     *  - previous, the previous vertex on the shortest path.
     *
     * Return:
     *  N/A
     */
    void DijkstraShortestPath(const vertex_t s,
                              std::vector<weight_t> &dist,
                              std::vector<vertex_t> &previous);

    inline int VertexCounts() const { return m_adjlist.size();};

    inline void SetHeapType(HeapType type)
    {
        m_dijkstra_heap_type = type;
    }

    virtual ~Graph() {};

private:
    adjacency_list_t m_adjlist;
    HeapType m_dijkstra_heap_type;
    int m_num_edges;

private:
    void d_fib_heap(const vertex_t s,
                    std::vector<weight_t> &dist,
                    std::vector<vertex_t> &previous);

    void d_binomial_heap(const vertex_t s,
                         std::vector<weight_t> &dist,
                         std::vector<vertex_t> &previous);

    void d_bin_heap(const vertex_t s,
                    std::vector<weight_t> &dist,
                    std::vector<vertex_t> &previous);

    void d_std_set(const vertex_t s,
                     std::vector<weight_t> &dist,
                     std::vector<vertex_t> &previous);
};

#endif
