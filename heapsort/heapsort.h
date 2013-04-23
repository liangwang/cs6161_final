/*
 * CS6161 UVa CS Advanced Algorithm Course Project
 * Heap sort with different heaps.
 *
 * Author: Wei Wang (wwang@virginia.edu)
 */
#ifndef __ALGORITHM_PROJECT_HEAP_SORT__
#define __ALGORITHM_PROJECT_HEAP_SORT__

#include <boost/heap/priority_queue.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>

typedef unsigned long long int DATA_TYPE;
typedef boost::heap::fibonacci_heap<DATA_TYPE, 
	boost::heap::compare<std::greater<DATA_TYPE> > > FIB_HEAP;
typedef boost::heap::d_ary_heap<DATA_TYPE, boost::heap::arity<2>,
	boost::heap::compare<std::greater<DATA_TYPE> > >  BIN_HEAP;

enum heap_types{
	Fibonacci,
	Binomial,
	Binary,
	PriorityQueue,
	NumHeaps
};

class heap_sorter{
 private:
	DATA_TYPE *data; /* pointer to the originial array */
	FIB_HEAP *fib_heap;
	BIN_HEAP *bin_heap;
	unsigned long long len; 
 public: 
	/* constructor */
	heap_sorter(enum heap_types heap_type);
	/* destructor */
	~heap_sorter();

	/* contruct the heap 
	 * Input parameter:
	 *       in_data: Array of input data
	 *       in_len: number of elements in the array
	 * Return value:
	 *       0: success
         *       1: failed
	 */
	int bin_heapify(DATA_TYPE *in_data, unsigned long long in_len);
	int fib_heapify(DATA_TYPE *in_data, unsigned long long in_len);
	/* do the sort 
	 * Input parameter:
	 *       out_data: Array that holds the sorted data
	 * Return value:
	 *       0: success
         *       1: failed
	 */
	int bin_sort(DATA_TYPE *out_data);
	int fib_sort(DATA_TYPE *out_data);
};

#endif
