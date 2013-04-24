#ifndef _MSTGENERATOR_H
#define _MSTGENERATOR_H

#include "graph.hh"

using namespace std;

class MstGenerator{
	vector<vertex_t> prev;
	HeapPolicy hp;
		
public:
	// constructor
	MstGenerator(Graph &g, HeapPolicy hp);
	
	template <class Heap> 
	void mstGenerateHelper(Graph &g);
	
	void mstGenerate(Graph &g);
	
	void mstPrint();
	
};
	
#endif
