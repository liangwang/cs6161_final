#include <iostream>
#include <set>
#include <fstream>
#include <cassert>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include "graph.hh"

Graph::Graph()
{
    m_dijkstra_heap_type = Fibonacci;
    m_num_edges = 0;
}

void Graph::Build(const char *filename)
{
    int num_vertexs;
    vertex_t x, y;
    weight_t w;

    std::ifstream ifs(filename);

    if (!ifs) {
        std::cerr << "Failed to open "<< filename << std::endl;
        return;
    }

    ifs >> num_vertexs;

    m_adjlist.clear();
    m_adjlist.resize(num_vertexs);

    m_num_edges = 0;

    while (ifs.good()) {
        ifs >>x >>y >>w;
        if (x >= num_vertexs || y >= num_vertexs) {
            std::cerr<<"Error: index of vertex exceeds the maximum, x:"<<x << ", y: " << y << ". Line skipped." << std::endl;
            continue;
        }
        neighbor_t n(y, w);
        m_adjlist[x].push_back(n);
        m_num_edges ++;
    }
}

void Graph::Build(const std::string &filename)
{
    Build(filename.c_str());
}


bool Graph::Adjacent(const int x, const int y) const
{
    /* not acutally implemented yet */
    std::cerr << "Adjacent method is not implemented!" << std::endl;
    return true;
}


void Graph::DijkstraShortestPath(const vertex_t s,
                                 std::vector<weight_t> &dist,
                                 std::vector<vertex_t> &previous)
{
    switch(m_dijkstra_heap_type) {
    case Fibonacci:
        d_fib_heap(s, dist, previous);
        break;
    case Binomial:
        d_binomial_heap(s, dist, previous);
        break;
    case Binary:
        d_bin_heap(s, dist, previous);
        break;
    case PriorityQueue:
        d_std_set(s, dist, previous);
        break;
    default:
        d_fib_heap(s, dist, previous);
    }
}

void Graph::d_fib_heap(const vertex_t s,
                       std::vector<weight_t> &dist,
                       std::vector<vertex_t> &previous)
{
    int n = m_adjlist.size();
    assert(s < n);
    dist.clear();
    dist.resize(n, INF_WEIGHT);
    dist[s] = 0;

    previous.clear();
    previous.resize(n, VERTEX_UNDEFINED);

    typedef std::pair<weight_t, vertex_t> queue_entry_t;
    typedef boost::heap::fibonacci_heap<queue_entry_t,
               boost::heap::compare<std::greater<queue_entry_t> > > queue_t;
    queue_t vertex_queue;
    typedef queue_t::handle_type handle_t;
    std::vector<handle_t> handles;

    for (int iter = 0; iter < n; iter++){
        handles.push_back(vertex_queue.push(std::make_pair(dist[iter], iter)));
    }

    while(!vertex_queue.empty())
    {
        const std::pair<weight_t, vertex_t> top = vertex_queue.top();
        weight_t d = top.first;
        vertex_t u = top.second;
        vertex_queue.pop();

        const std::vector<neighbor> &neighbors = m_adjlist[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = d + weight;
            if (distance_through_u < dist[v]) {
                dist[v] = distance_through_u;
                previous[v] = u;

                vertex_queue.update(handles[v], std::make_pair(dist[v],v));

            }
        }
    }
}

void Graph::d_std_set(const vertex_t s,
                        std::vector<weight_t> &dist,
                        std::vector<vertex_t> &previous)
{
    int n = m_adjlist.size();
    dist.clear();
    dist.resize(n, INF_WEIGHT);
    dist[s] = 0;

    previous.clear();
    previous.resize(n, VERTEX_UNDEFINED);

    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
    vertex_queue.insert(std::make_pair(dist[s], s));

    while (!vertex_queue.empty())
    {
        weight_t d = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        const std::vector<neighbor> &neighbors = m_adjlist[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter ++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = d + weight;
            if (distance_through_u < dist[v]) {
                vertex_queue.erase(std::make_pair(dist[v],v));

                dist[v] = distance_through_u;
                previous[v] = u;
                vertex_queue.insert(std::make_pair(dist[v], v));
            }
        }
    }
}


void Graph::d_binomial_heap(vertex_t s,
                     std::vector<weight_t> &dist,
                     std::vector<vertex_t> &previous)
{
    int n = m_adjlist.size();
    assert(s < n);
    dist.clear();
    dist.resize(n, INF_WEIGHT);
    dist[s] = 0;

    previous.clear();
    previous.resize(n, VERTEX_UNDEFINED);

    typedef std::pair<weight_t, vertex_t> queue_entry_t;
    typedef boost::heap::binomial_heap<queue_entry_t,
               boost::heap::compare<std::greater<queue_entry_t> > > queue_t;
    queue_t vertex_queue;
    typedef queue_t::handle_type handle_t;
    std::vector<handle_t> handles;

    for (int iter = 0; iter < n; iter++){
        handles.push_back(vertex_queue.push(std::make_pair(dist[iter], iter)));
    }

    while(!vertex_queue.empty())
    {
        const std::pair<weight_t, vertex_t> top = vertex_queue.top();
        weight_t d = top.first;
        vertex_t u = top.second;
        vertex_queue.pop();

        const std::vector<neighbor> &neighbors = m_adjlist[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = d + weight;
            if (distance_through_u < dist[v]) {

                dist[v] = distance_through_u;
                previous[v] = u;

                vertex_queue.increase(handles[v], std::make_pair(dist[v],v));

            }
        }
    }
}

void Graph::d_bin_heap(vertex_t s,
                       std::vector<weight_t> &dist,
                       std::vector<vertex_t> &previous)
{
    int n = m_adjlist.size();
    assert(s < n);
    dist.clear();
    dist.resize(n, INF_WEIGHT);
    dist[s] = 0;

    previous.clear();
    previous.resize(n, VERTEX_UNDEFINED);

    typedef std::pair<weight_t, vertex_t> queue_entry_t;
    typedef boost::heap::d_ary_heap<queue_entry_t,
               boost::heap::mutable_<true>,boost::heap::arity<2>,
               boost::heap::compare<std::greater<queue_entry_t> > > queue_t;
    queue_t vertex_queue;
    typedef queue_t::handle_type handle_t;
    std::vector<handle_t> handles;

    for (int iter = 0; iter < n; iter++){
        handles.push_back(vertex_queue.push(std::make_pair(dist[iter], iter)));
    }

    while(!vertex_queue.empty())
    {
        const std::pair<weight_t, vertex_t> top = vertex_queue.top();
        weight_t d = top.first;
        vertex_t u = top.second;
        vertex_queue.pop();

        const std::vector<neighbor> &neighbors = m_adjlist[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = d + weight;
            if (distance_through_u < dist[v]) {

                dist[v] = distance_through_u;
                previous[v] = u;

                vertex_queue.increase(handles[v], std::make_pair(dist[v],v));

            }
        }
    }
}
