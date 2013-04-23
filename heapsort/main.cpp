/*
 * Comparing performance of different heaps using heap sort. This program takes 
 * a data file as input, sorts the data in the file, and outputs sort data.
 * Input files should be in the follow format: first line has total number of
 * the input data. After the first line, each line is a datum. The data can be
 * all positive integers or all positive floats.
 * Output file has the same name as input file with ".out" suffix.
 *
 * Author: Wei Wang (wwang@virginia.edu)
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <err.h>
#include <sys/time.h>

#include "heapsort.h"

char * file_name;
enum heap_types heap_type;
int debug;

int print_help()
{
	const char * help_strings[11] = {
		"Usage: heapsort [options]",
		"",
		"Options:",
		"  -h, --help",
		"\t show this help message and exit",
		"  -f FILENAME, --file=FILENAME",
		"\t name of the input file",
		"  -t HEAPTYPE, --heaptype=HEAPTYPE",
		"\t heap type: 0, Fibonacci; 1, Binomial; 2, Binary; "
		"3, PriorityQueue; 4, NumHeaps",
		"  -v, --verbose",
		"\t enable debug output.",
	};

	int i;

	for(i = 0; i < 11; i++)
		fprintf(stderr, "%s\n", help_strings[i]);

	return 0;
}


/*
 * Parse commandline parameters.
 * Function parameters are self-explanatory.
 * Returns 0 for success; non-zero means failure
 */
int parse_parameters(int argc, char *argv[])
{
	struct option long_options[] = {
		{"help", no_argument, NULL, 1001},
		{"heaptype", required_argument, NULL, 1002},
		{"file", required_argument, NULL, 1003},
		{"debug", no_argument, NULL, 1004}
	};
	
	int op;
	int long_op_index;
	char error_msg[256] = {0};

	/* initial values */
	file_name = NULL;
	heap_type = Fibonacci;
	debug = 0;
	
	/* parse the commandline parameters */
	while(1){
		op = getopt_long(argc, argv, "ht:f:d", long_options, 
				 &long_op_index);

		if(op == -1)
			break;
		
		switch(op){
		case 'h':
		case 1001:
			print_help();
		        exit(0);
		case 't':
		case 1002:
			heap_type = (enum heap_types)atoi(optarg);
		        break;
		case 'f':
		case 1003:
			file_name = optarg;
			break;
		case 'd':
		case 1004:
			debug = 1;
			break;
		default:
			sprintf(error_msg, "%s", "Unknown options or missing "
				"required value\n");
			goto error;
			break;
		}
	}

	if(debug)
		printf("Input parameters: heap type %d, file name %s\n",
		       heap_type, file_name);

	/* check whether we have enough parameters */
	if(file_name == NULL){
		sprintf(error_msg, "%s", "Please specify the input file "
			"name\n");
		goto error;
	}

	return 0;

 error:
	fprintf(stderr, "%s\n", error_msg);
	print_help();
	exit(1);
}

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

int main(int argc, char* argv[])
{
	DATA_TYPE *in_data;
	DATA_TYPE *out_data;
	heap_sorter * sorter;
	unsigned long long i;
	char out_f_name[1024] = {0};
	FILE *in_f, *out_f;
	unsigned long long len = 10;

	/* parse command line parameters */
	parse_parameters(argc, argv);

	/* read input data */
	in_f = fopen(file_name, "r");
	if(in_f == NULL)
		err(2, "Unable to open input file %s", file_name);
	fscanf(in_f, "%llu\n", &len);
	if(debug)
		printf("There are %llu input data\n", len);

	in_data = (DATA_TYPE*)malloc(sizeof(DATA_TYPE)*len);
	out_data = (DATA_TYPE*)malloc(sizeof(DATA_TYPE)*len);
	for(i = 0; i < len; i++)
		fscanf(in_f, "%llu\n", in_data+i);
	fclose(in_f);

	/* sort the array */
	switch(heap_type){
	case Fibonacci:
		sorter = new heap_sorter(heap_type);

		get_start_time();
		sorter->fib_heapify(in_data, len);
		get_end_time();
		get_elapsed_time();
		printf("Heapify time %llu us, %f s\n", elapsed_time, 
		       elapsed_time_sec);

		get_start_time();
		sorter->fib_sort(out_data);
		get_end_time();
		get_elapsed_time();
		printf("Sort time %llu us, %f s\n", elapsed_time, 
		       elapsed_time_sec);
		break;
	case Binary:
		sorter = new heap_sorter(heap_type);
		
		get_start_time();
		sorter->bin_heapify(in_data, len);
		get_end_time();
		get_elapsed_time();
		printf("Heapify time %llu us, %f s\n", elapsed_time, 
		       elapsed_time_sec);

		get_start_time();
		sorter->bin_sort(out_data);
		get_end_time();
		get_elapsed_time();
		printf("Sort time %llu us, %f s\n", elapsed_time, 
		       elapsed_time_sec);
		break;
	default:
		printf("Other heaps not implemented\n");
		return 1;
	}

	/* check the outputs */
	for(i = 1; i < len; i++){
		if(out_data[i]<out_data[i-1])
			printf("Error wrong output\n");
	}

	/* output the sorted data into a file */
	sprintf(out_f_name, "%s.out", file_name);
	out_f = fopen(out_f_name, "w");
	if(out_f == NULL)
		err(2, "Unable to open output file %s", out_f_name);
	fprintf(out_f, "%llu\n", len);
	for(i = 0; i < len; i++){
		fprintf(out_f, "%llu\n", out_data[i]);
	}
	fclose(out_f);

	/* clean up */
	free(in_data);
	free(out_data);
	
	delete sorter;
	
	return 0;
}
