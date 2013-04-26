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
parser.add_option("--stripped", action="store_true", dest="stripped", 
                  default=False, help="Parse results for stripped version")
parser.add_option("--heapify", action="store_true", dest="heapify", 
                  default=False, help="Parse results for stripped version")


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
if options.stripped and options.heapify:
    print "Only one of stripped and heapify can be set."
    parser.print_help()
    exit(-1)

# open the log file for this heap, size and data type
if options.stripped:
    file_name = (options.data_type + "_pfm/" + options.data_type + "_" + 
                 options.size + "_" + options.heap_type + "_pfm_strp_log.txt")
elif options.heapify:
    file_name = (options.data_type + "_pfm/" + options.data_type + "_" + 
                 options.size + "_" + options.heap_type + "_pfm_hpy_log.txt")
else:
    file_name = (options.data_type + "_pfm/" + options.data_type + "_" + 
                 options.size + "_" + options.heap_type + "_pfm_log.txt")

log_f = open(file_name, "r")

# convert the data set size to integer value
data_len = parseMemSize(options.size)
array_size = data_len * 8 * 2 # this is the size of the arrays I used in my
                              # program                   

# process the log and get the execution time and memory usage
insns = {} 
l1_misses = {}
l2_misses = {}
l3_misses = {}

cur_set = -1
cur_iter = -1
for line in log_f:
    if line.startswith("Begin_data_set"): # a new data set
        cur_set = int(line.split(' ')[1])
        cur_iter = int(line.split(' ')[3].rstrip('\n'))
        continue

    if "DATA_CACHE_MISSES" in line:
        l1m_s = line.split("DATA_CACHE_MISSES")[0].split(':')[1].strip()
        l1m = int(l1m_s.replace(',', ''))
        if not (cur_set in l1_misses): # no entry for this data set
            l1_misses[cur_set] = {} # each element in turn has results for
                                       # each iteration
        l1_misses[cur_set][cur_iter] = l1m
    
    if "L2_CACHE_MISS:ALL" in line:
        l2m_s = line.split("L2_CACHE_MISS:ALL")[0].split(':')[1].strip()
        l2m = int(l2m_s.replace(',', ''))
        if not (cur_set in l2_misses): # no entry for this data set
            l2_misses[cur_set] = {} # each element in turn has results for
                                       # each iteration
        l2_misses[cur_set][cur_iter] = l2m

    if "DRAM_ACCESSES_PAGE:ALL" in line:
        l3m_s = line.split("DRAM_ACCESSES_PAGE:ALL")[0].split(':')[1].strip()
        l3m = int(l3m_s.replace(',', ''))
        if not (cur_set in l3_misses): # no entry for this data set
            l3_misses[cur_set] = {} # each element in turn has results for
                                       # each iteration
        l3_misses[cur_set][cur_iter] = l3m

    if "PERF_COUNT_HW_INSTRUCTION" in line:
        insn_s = line.split("PERF_COUNT_HW_INSTRUCTION")[0].split(':')[1]
        insn = int(insn_s.strip().replace(',', ''))
        if not (cur_set in insns): # no entry for this data set
            insns[cur_set] = {} # each element in turn has results for
                                       # each iteration
        insns[cur_set][cur_iter] = insn


if options.verbose:
    for d in insns:
        print "Data set:", d
        for i in insns[d]:
            print "   iter:", i, "insn:", insns[d][i],
            print "l1m:", l1_misses[d][i],
            print "l2m:", l2_misses[d][i],
            print "l3m:", l3_misses[d][i],


# generate average time and memory size for each data set
# for easy parsing outputs are space separated lines with following columns
# index index2 insn_cnt l1_misses l2_misses l3_misses
if options.verbose:
    print "data_set X insn_cnt l1_misses l2_misses l3_misses",
all_insns = []
all_l1ms = []
all_l2ms = []
all_l3ms = []
for d in insns:
    ins = []
    l1ms = []
    l2ms = []
    l3ms = []
    for i in insns[d]:
        ins.append(insns[d][i])
        l1ms.append(l1_misses[d][i])
        l2ms.append(l2_misses[d][i])
        l3ms.append(l3_misses[d][i])

        all_insns.append(insns[d][i])
        all_l1ms.append(l1_misses[d][i])
        all_l2ms.append(l2_misses[d][i])
        all_l3ms.append(l3_misses[d][i])

    if options.verbose:
        avg_insns = numpy.average(ins)
        std_insns = numpy.std(ins)
        
        avg_l1ms = numpy.average(l1ms)
        std_l1ms = numpy.std(l1ms)
        
        avg_l2ms = numpy.average(l2ms)
        std_l2ms = numpy.std(l2ms)
        
        avg_l3ms = numpy.average(l3ms)
        std_l3ms = numpy.std(l3ms)

        print d, "", avg_insns, avg_l1ms, avg_l2ms, avg_l3ms

# get overall averages
avg_insns = numpy.average(all_insns)
std_insns = numpy.std(all_insns)

avg_l1ms = numpy.average(all_l1ms)
std_l1ms = numpy.std(all_l1ms)

avg_l2ms = numpy.average(all_l2ms)
std_l2ms = numpy.std(all_l2ms)

avg_l3ms = numpy.average(all_l3ms)
std_l3ms = numpy.std(all_l3ms)

print options.size, data_len, avg_insns, avg_l1ms, avg_l2ms, avg_l3ms
