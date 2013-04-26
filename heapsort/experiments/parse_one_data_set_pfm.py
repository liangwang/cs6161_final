#!/usr/bin/python

from optparse import OptionParser
import resource
import subprocess
import numpy

# Parse all data sets for one heap and one data set type
# define command line parameters
parser = OptionParser()
parser.add_option("-t", "--type", dest="heap_type", 
                  help="Type of the heap, can be fib or bin.", metavar="HEAP")
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
if options.data_type is None:
    print "Please specify the type of data."
    parser.print_help()
    exit(-1)

data_set_sizes = ["10", "100", "1K", "8K", "10K", "16K", "64K", "100K", 
                  "128K", "625K", "1M", "2M", "5M"]

print "size size instructions l1_misses l2_misses l3_misses"
for s in data_set_sizes:
    command = ("./parse_results_pfm.py -t " + options.heap_type + " -d " + 
               options.data_type + " -s " + s)
    subprocess.check_call(command, shell=True)

print "size size insn_base l1m_base l2m_base l3m_base"
for s in data_set_sizes:
    command = ("./parse_results_pfm.py -t " + options.heap_type + " -d " + 
               options.data_type + " -s " + s + " --stripped")
    subprocess.check_call(command, shell=True)

print "size size insn_hpy l1m_hpy l2m_hpy l3m_hpy"
for s in data_set_sizes:
    command = ("./parse_results_pfm.py -t " + options.heap_type + " -d " + 
               options.data_type + " -s " + s + " --heapify")
    subprocess.check_call(command, shell=True)
