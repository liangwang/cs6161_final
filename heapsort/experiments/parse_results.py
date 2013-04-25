#!/usr/bin/python

# Parse the results of the heap experiments, generate the average execution time
# and memory usage for one heap type and one memory size and one kind of data
# sets

from optparse import OptionParser
import resource
import subprocess
import numpy

# This function parse a string which represents memory
# size such as "xxB", "xxK", "xxKB" ... "xxGB"
# Parameters:
#     str memsize: the memory size string
# Return:
#     int size: memory size in bytes
def parseMemSize( memsize):
    strLen = len(memsize)
    lastChar = memsize[strLen-1]

    if lastChar == 'B':
        lastChar = memsize[strLen - 2]
        memsize = memsize[:-1]
    
    if lastChar == 'G':
        unit = 1024*1024*1024
        memsize = memsize[:-1]
    elif lastChar == 'M':
        unit = 1024*1024
        memsize = memsize[:-1]
    elif lastChar == 'K':
        unit = 1024
        memsize = memsize[:-1]
    else:
        unit = 1

    try:
        size = int(memsize,10) * unit
        return size
    except ValueError:
        return -1

# define command line parameters
parser = OptionParser()
parser.add_option("-t", "--type", dest="heap_type", 
                  help="Type of the heap, can be fib or bin.", metavar="HEAP")
parser.add_option("-s", "--size", dest="size", 
                  help="Size of the data set.", metavar="SIZE")
parser.add_option("-d", "--data", dest="data_type", 
                  help="Data type, can be rand, sorted, rsorted or same", 
                  metavar="DATA_TYPE")
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", 
                  default=False, help="Enable debug output")

(options, args) = parser.parse_args()

# check command line parameters
if options.heap_type is None:
    print "Please specify heap type."
    parser.print_help()
    exit(-1)
if options.size is None:
    print "Please specify data set size."
    parser.print_help()
    exit(-1)
if options.data_type is None:
    print "Please specify the type of data."
    parser.print_help()
    exit(-1)

# open the log file for this heap, size and data type
file_name = (options.data_type + "/" + options.data_type + "_" + options.size + 
             "_" + options.heap_type + "_log.txt")
log_f = open(file_name, "r")

# convert the data set size to integer value
data_len = parseMemSize(options.size)
array_size = data_len * 8 * 2 # this is the size of the arrays I used in my
                              # program                   

# process the log and get the execution time and memory usage
heapify_time = {} # heapify time, each element has the result of one data set
                  # in the experiments, I have 10 data sets, and each data
                  # set is repeatedly executed for 3 times.
sort_time = {} # same as heapify time 
mem_size = {}  # memory used, after removing the memory used for in/out arrays
org_mem_size = {} # original memory used, including all data structures

cur_set = -1
cur_iter = -1
for line in log_f:
    if line.startswith("Begin_data_set"): # a new data set
        cur_set = int(line.split(' ')[1])
        cur_iter = int(line.split(' ')[3].rstrip('\n'))
        continue

    if line.startswith("Heapify time"):
        htime = float(line.split(' ')[4]) # get the heapify time
        if not (cur_set in heapify_time): # no entry for this data set
            heapify_time[cur_set] = {} # each element in turn has results for
                                       # each iteration
        heapify_time[cur_set][cur_iter] = htime;
    
    if line.startswith("Sort time"):
        stime = float(line.split(' ')[4]) # get the sort time
        if not (cur_set in sort_time): # no entry for this data set
            sort_time[cur_set] = {}
        sort_time[cur_set][cur_iter] = stime;

    if line.lstrip().startswith("Maximum resident set size"):
        mem = int(line.split(': ')[1].rstrip('\n')) * 1024 # get the mem size
        mem = mem / 4                                   # there is a bug in 
                                                        # GNU time, with reports
                                                        # 4 times more memory
        if not (cur_set in mem_size): # no entry for this data set
            mem_size[cur_set] = {}
            org_mem_size[cur_set] = {}
        mem_size[cur_set][cur_iter] = mem - array_size;
        org_mem_size[cur_set][cur_iter] = mem;

if options.verbose:
    for d in heapify_time:
        print "Data set:", d
        for i in heapify_time[d]:
            print "   iter:", i, "heapify:", heapify_time[d][i],
            print "sort:", sort_time[d][i],
            print "mem:", mem_size[d][i], "org mem:", org_mem_size[d][i]

# generate average time and memory size for each data set
# for easy parsing outputs are space separated lines with following columns
# index index2 heapify_time sort_time mem org_mem heapify_std_div sort_std_div
# mem_std_div org_mem_std_div
if options.verbose:
    print "data_set X heapify_time sort_time total_time mem org_mem",
    print "heapify_std_div",
    print "sort_std_div total_std_div mem_std_div org_mem_std_div"
all_htimes = []
all_stimes = []
all_ttimes = []
all_mem_sizes = []
all_org_mem_sizes = []
for d in heapify_time:
    htimes = []
    stimes = []
    ttimes = []
    mem_sizes = []
    org_mem_sizes = []
    for i in heapify_time[d]:
        htimes.append(heapify_time[d][i])
        stimes.append(sort_time[d][i])
        ttimes.append(heapify_time[d][i]+sort_time[d][i])
        mem_sizes.append(mem_size[d][i])
        org_mem_sizes.append(org_mem_size[d][i])
        
        all_htimes.append(heapify_time[d][i])
        all_stimes.append(sort_time[d][i])
        all_ttimes.append(heapify_time[d][i]+sort_time[d][i])
        all_mem_sizes.append(mem_size[d][i])
        all_org_mem_sizes.append(org_mem_size[d][i])

    if options.verbose:
        avg_htime = numpy.average(htimes)
        std_htime = numpy.std(htimes)
        
        avg_stime = numpy.average(stimes)
        std_stime = numpy.std(stimes)
        
        avg_ttime = numpy.average(ttimes)
        std_ttime = numpy.std(ttimes)
        
        avg_mem = numpy.average(mem_sizes)
        std_mem = numpy.std(mem_sizes)

        avg_omem = numpy.average(org_mem_sizes)
        std_omem = numpy.std(org_mem_sizes)
        print d, "", avg_htime, avg_stime, avg_ttime, avg_mem, avg_omem, 
        print std_htime, std_stime, std_ttime, std_mem, std_omem

# get overall averages
avg_htime = numpy.average(all_htimes)
std_htime = numpy.std(all_htimes)

avg_stime = numpy.average(all_stimes)
std_stime = numpy.std(all_stimes)
        
avg_ttime = numpy.average(all_ttimes)
std_ttime = numpy.std(all_ttimes)
        
avg_mem = numpy.average(all_mem_sizes)
std_mem = numpy.std(all_mem_sizes)
        
avg_omem = numpy.average(all_org_mem_sizes)
std_omem = numpy.std(all_org_mem_sizes)

print options.size, data_len, avg_htime, avg_stime, avg_ttime, avg_mem, 
print avg_omem, std_htime, std_stime, std_ttime, std_mem, std_omem
