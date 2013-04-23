#include <stdio.h>
#include <stdlib.h>

#include <boost/heap/priority_queue.hpp>
#include <boost/heap/fibonacci_heap.hpp>

#include "heapsort.h"

/* constructor */
heap_sorter::heap_sorter(enum heap_types heap_type){
	/* set NULL pointers */
	data = NULL;
	fib_heap = NULL;
	bin_heap = NULL;

	switch(heap_type){
	case Fibonacci:
		fib_heap = new FIB_HEAP;
		break;
	case Binary:
		bin_heap = new BIN_HEAP;
	case PriorityQueue:
	case NumHeaps:
	case Binomial:
		fib_heap = NULL;
	}

	return;
}

/* destructor */
heap_sorter::~heap_sorter(){
	if(fib_heap != NULL)
		delete fib_heap;

	return;
}

/* construct the heap from the input data array */
int heap_sorter::fib_heapify(DATA_TYPE *in_data, unsigned long long in_len){
	unsigned long long i;

	len = in_len;
	data = in_data;
	
	for(i = 0; i < len; i++)
		fib_heap->push(in_data[i]);
	
	return 0;
}

/* sort the data and put the sorted data in the out_data array */
int heap_sorter::fib_sort(DATA_TYPE *out_data){
	unsigned long long index = 0; 
	
	while(fib_heap->size()){
		out_data[index++] = fib_heap->top(); /* get the smallest one 
							form the heap */
		fib_heap->pop(); /* remove the smallest one form the heap */
	}

	return 0;
}

/* construct the heap from the input data array */
int heap_sorter::bin_heapify(DATA_TYPE *in_data, unsigned long long in_len){
	unsigned long long i;

	len = in_len;
	data = in_data;
	
	for(i = 0; i < len; i++)
		bin_heap->push(in_data[i]);
	
	return 0;
}

/* sort the data and put the sorted data in the out_data array */
int heap_sorter::bin_sort(DATA_TYPE *out_data){
	unsigned long long index = 0; 
	
	while(bin_heap->size()){
		out_data[index++] = bin_heap->top(); /* get the smallest one 
							form the heap */
		bin_heap->pop(); /* remove the smallest one form the heap */
	}

	return 0;
}
