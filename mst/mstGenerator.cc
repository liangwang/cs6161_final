#include <iostream>
#include <boost/heap/priority_queue.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include "mstGenerator.hh"
#include "graph.hh"

using namespace std;

struct vertex
{
	int index;
	float weight;
	
	explicit vertex(int i, float w): index(i), weight(w) {}
	bool operator< (vertex const &newVertex) const {return weight > newVertex.weight; }
};

MstGenerator::MstGenerator(Graph &g, HeapPolicy hp)
{
	int numOfVertices = g.m_adjlist.size();
	
	// init prev
	int i;
	for(i=0; i<numOfVertices; i++)
	{
		prev.push_back(-1);
	}
	
	this->hp = hp;
}

template <class Heap> 
void MstGenerator::mstGenerateHelper(Graph &g)
{
	Heap heap;
	vector<typename Heap::handle_type> reference;
	vector<float> rest;
	
	// init vertex 0 to be the start vertex
	reference.push_back(heap.push(vertex(0, 0)));
	rest.push_back(0);
	
	int i;
	for(i=1; i<g.m_adjlist.size(); i++)
	{
		reference.push_back(heap.push(vertex(i, INT_MAX)));
		rest.push_back(INT_MAX);
	}
	
	int j;
	while(!heap.empty())
	{
		vertex curr= heap.top();
		heap.pop();
		rest[curr.index] = -1;
		
		for(j=0; j<g.m_adjlist[curr.index].size(); j++)
		{
			neighbor adj = g.m_adjlist[curr.index][j];
			if(rest[adj.target] != -1 && adj.weight < rest[adj.target])
			{
				// decrease key in heap
				heap.update(reference[adj.target], vertex(adj.target, adj.weight));
				
				// update result
				rest[adj.target] = adj.weight;
				prev[adj.target] = curr.index;
			}
		}
	}
}


void MstGenerator::mstGenerate(Graph &g)
{
	switch(hp)
	{
		// TODO: More Heap types to be added...
		case Fibonacci:
		mstGenerateHelper<boost::heap::fibonacci_heap<vertex> > (g);
		break;
		case Binomial:
		mstGenerateHelper<boost::heap::binomial_heap<vertex> > (g);
		break;
		case Binary:
		mstGenerateHelper<boost::heap::d_ary_heap<vertex, boost::heap::arity<2>, boost::heap::mutable_<true> > > (g);
		break;
		case PriorityQueue:
	//	mstGenerateHelper<boost::heap::priority_queue<vertex> >(g);
		break;
	}
}

void MstGenerator::mstPrint()
{
	// print each edge in MST
	int i;
	for(i=0; i<prev.size(); i++)
	{
		cout<<prev[i]<<" "<<i<<endl;
	}
}
