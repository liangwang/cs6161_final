#include <iostream>
#include <vector>
#include <sys/time.h>

#include "graph.hh"
#include "mstGenerator.hh"

using namespace std;

struct timeval start, end;
unsigned long long elapsed_time; /* in micro seconds */
double elapsed_time_sec; /* in seconds */

int get_start_time(){
	return gettimeofday(&start, NULL);
}

int get_end_time(){
	return gettimeofday(&end, NULL);
}

int get_elapsed_time(){
	elapsed_time = 1000000 * (end.tv_sec - start.tv_sec) + 
		(end.tv_usec - start.tv_usec);

	elapsed_time_sec = ((double)elapsed_time) / 1000000;

	return 0;
}

int main()
{
	Graph g;
    g.Build();
	
	MstGenerator mst(g, Fibonacci);
	
	get_start_time();
	mst.mstGenerate(g);
	get_end_time();
	get_elapsed_time();
	cout<<"Time cost: "<<elapsed_time_sec<<" s!"<<endl;
	
	mst.mstPrint();
	
	return 0;
}
