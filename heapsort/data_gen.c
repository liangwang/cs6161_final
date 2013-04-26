/*
 * This program generates a data file for the heap sort project.
 * The data file is in the follow format: first line has total number of
 * elements. After the first line, each line is an element. The elements can be
 * all positive integers or all positive floats.
 *
 * Author: Wei Wang (wwang@virginia.edu)
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <err.h>
#include <sys/time.h>

unsigned long long len;
char * file_name;
int debug;
int one_number;

int print_help()
{
	const char * help_strings[13] = {
		"Usage: heapsort [options]",
		"",
		"Options:",
		"  -h, --help",
		"\t show this help message and exit",
		"  -f FILENAME, --file=FILENAME",
		"\t name of the output file",
		"  -l LENGTH, --length=LENGTH",
		"\t how name elements to generate ",
		"  -s, --samenumber",
		"\t generate LENGTH copies of one random number",
		"  -v, --verbose",
		"\t enable debug output.",
	};

	int i;

	for(i = 0; i < 13; i++)
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
		{"length", required_argument, NULL, 1002},
		{"file", required_argument, NULL, 1003},
		{"debug", no_argument, NULL, 1004},
		{"samenumber", no_argument, NULL, 1005}
	};
	
	int op;
	int long_op_index;
	char error_msg[256] = {0};

	/* initial values */
	file_name = NULL;
	len = 0;
	debug = 0;
	one_number = 0;
	
	/* parse the commandline parameters */
	while(1){
		op = getopt_long(argc, argv, "hl:f:ds", long_options, 
				 &long_op_index);

		if(op == -1)
			break;
		
		switch(op){
		case 'h':
		case 1001:
			print_help();
		        exit(0);
		case 'l':
		case 1002:
			len = strtoull(optarg, NULL, 0);
		        break;
		case 'f':
		case 1003:
			file_name = optarg;
			break;
		case 'd':
		case 1004:
			debug = 1;
			break;
		case 's':
		case 1005:
			one_number = 1;
			break;
		default:
			sprintf(error_msg, "%s", "Unknown options or missing "
				"required value\n");
			goto error;
			break;
		}
	}

	if(debug)
		printf("Input parameters: length %llu, file name %s,"
		       "same number %d\n", 
		       len, file_name, one_number);

	/* check whether we have enough parameters */
	if(file_name == NULL){
		sprintf(error_msg, "%s", "Please specify the output file "
			"name\n");
		goto error;
	}
	if(len == 0){
		sprintf(error_msg, "%s", "Please specify the length of output "
			"data\n");
		goto error;
	}


	return 0;

 error:
	fprintf(stderr, "%s\n", error_msg);
	print_help();
	exit(1);
}

int gen_one_number(unsigned long long *out)
{
	long a = random();
	long b = random();
	
	*out = a << 32; // put a on the first half
	*out = *out | b; // put b on the second half
	
	return 0;
}

int main(int argc, char* argv[])
{
	unsigned long long i;
	FILE *out_f;
	unsigned long long element;

	/* parse command line parameters */
	parse_parameters(argc, argv);

	/* generate data */
	out_f = fopen(file_name, "w");
	if(out_f == NULL)
		err(2, "Unable to open output file %s", file_name);
	fprintf(out_f, "%llu\n", len);
	gen_one_number(&element);
	for(i = 0; i < len; i++){
		if(!one_number)
			gen_one_number(&element);
		fprintf(out_f, "%llu\n", element);
	}
	fclose(out_f);

	return 0;
}
