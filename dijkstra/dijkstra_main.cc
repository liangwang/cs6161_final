#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <sys/time.h>

#include "graph.hh"

char * file_name = NULL;
enum HeapType heap_type = Fibonacci;
int debug = 0;


int print_help(char ** argv)
{
    std::cerr << "Usage: " << argv[0] << " [options]\n";
    std::cerr << "Options:\n";
	std::cerr << "  -h, --help\n";
	std::cerr << "    show this help message and exit\n";
	std::cerr << "  -f FILENAME, --file=FILENAME\n";
	std::cerr << "    name of the input file\n";
	std::cerr << "  -t HEAPTYPE, --heaptype=HEAPTYPE\n";
	std::cerr << "    heap type: 0, Fibonacci; 1, Binomial; 2, Binary; \n";
	std::cerr << "    3, PriorityQueue\n";
	std::cerr << "  -v, --verbose\n";
	std::cerr << "    enable debug output.\n";

    return EXIT_FAILURE;
}

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
			print_help(argv);
		        exit(0);
		case 't':
		case 1002:
			heap_type = (enum HeapType)atoi(optarg);
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
            std::cerr<<"Unknown options or missing required value" << std::endl;
            print_help(argv);
            exit(1);
		}
	}

    if (heap_type >= NumHeaps) {
        std::cerr<<"Unkown heap type" << std::endl;
        print_help(argv);
        exit(1);
    }

    if (file_name == NULL ) {
        std::cerr<< "Please specify the input file name" << std::endl;
        print_help(argv);
        exit(1);
    }


	if(debug)
        std::cout<<"Input parameters: heap type "<<heap_type << ", file name "<< file_name << std::endl;

	return 0;

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



int main(int argc, char** argv)
{
    Graph g;

	/* parse command line parameters */
	parse_parameters(argc, argv);

    g.Build(file_name);
    int num_vertex = g.VertexCounts();

    srand((unsigned)time(0));
    int s = rand() % num_vertex;

    std::vector<weight_t> dist;
    std::vector<vertex_t> previous;

    g.SetHeapType(heap_type);

    std::cout<<">>>>> Compute the shortest path from node "<<s << std::endl;
    get_start_time();
    g.DijkstraShortestPath(s, dist, previous);
    get_end_time();
    get_elapsed_time();

    std::cout<<"Time "<< elapsed_time <<" us, " << elapsed_time_sec << " s" << std::endl;

    return EXIT_SUCCESS;
}
