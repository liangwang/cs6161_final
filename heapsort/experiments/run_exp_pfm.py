#!/usr/bin/python

import subprocess
import os.path
import shutil

def convert(val):
    lookup = {'K': 1000, 'M': 1000000, 'B': 1000000000}
    unit = val[-1]
    try:
        number = int(val[:-1])
    except ValueError:
        print "ValueError"
        exit(1)

    if unit in lookup:
        return lookup[unit] * number
    return int(val)

iters = 1 #how many iters for one data set
#cases = ["rand", "same", "sorted", "rsorted"]
cases = ["rand"]
data_set_sizes = ["10", "100", "1K", "8K", "10K", "16K", "64K", "100K", 
                  "128K", "625K", "1M", "2M", "5M"]
#data_set_sizes = ["10M", "100M", "1B"]
sets_per_size = 1 # how many data sets to run for each size
#data_set_sizes = ["10"]
heaps = {"fib":0, "bin":2} # names and indices of the heaps to testa

pfm_cmd = ("/if14/ww6r/WorkingDisk/Research/workrepo/Programs/pfm_tools/pfm_multi -e " + 
           "DATA_CACHE_MISSES,L2_CACHE_MISS:ALL,DRAM_ACCESSES_PAGE:ALL," + 
           "PERF_COUNT_HW_INSTRUCTIONS ")
heapsort_cmd = "./heap_sort"
# data sets in random order
case = 0
for s in data_set_sizes:
    length = convert(s)
    for idx in range(sets_per_size):
        #generate data set file
        ds_fn = cases[case] + "_" + s + "_" + str(idx) + ".txt"
        subprocess.check_call(["./data_gen", "-f" + ds_fn, 
                               "-l" + str(length)])
        #sort the data set
        for heap in heaps:
            log_fn = cases[case] + "_" + s + "_" + heap + "_pfm_log.txt"
            log_f = open(log_fn, 'a+')
            for i in range(iters):
                log_f.write("Begin_data_set " + str(idx) + " iter " + str(i) + 
                            "\n")
                log_f.flush()
                subprocess.check_call([pfm_cmd +  
                                       heapsort_cmd + " -f " + ds_fn +  
                                       " -t " + str(heaps[heap]) + " -d"], 
                                      stdout = log_f, stderr = log_f, 
                                      shell = True)
                log_f.flush()
            log_f.close()
                               

# # data sets with only one number
# case = 1
# for s in data_set_sizes:
#     length = convert(s)
#     for idx in range(sets_per_size):
#         #generate data set file
#         ds_fn = cases[case] + "_" + s + "_" + str(idx) + ".txt"
#         subprocess.check_call(["./data_gen", "-f" + ds_fn, "-s", 
#                                "-l" + str(length)])
#         #sort the data set
#         for heap in heaps:
#             log_fn = cases[case] + "_" + s + "_" + heap + "_log.txt"
#             log_f = open(log_fn, 'a+')
#             for i in range(iters):
#                 log_f.write("Begin_data_set " + str(idx) + " iter " + str(i) + 
#                             "\n")
#                 log_f.flush()
#                 subprocess.check_call(["/usr/bin/time -v " +  
#                                        "./heapsort -f " + ds_fn +  
#                                        " -t " + str(heaps[heap]) + " -d"], 
#                                       stdout = log_f, stderr = log_f, 
#                                       shell = True)
#                 log_f.flush()
#             log_f.close()
        

# # data sets with sorted numbers
# case = 2
# for s in data_set_sizes:
#     length = convert(s)
#     for idx in range(sets_per_size):
#         #generate data set file
#         ds_fn_s = cases[0] + "_" + s + "_" + str(idx) + ".txt.out"
#         ds_fn = cases[case] + "_" + s + "_" + str(idx) + ".txt"
#         if not os.path.isfile(ds_fn_s):
#             print " Sorted file", ds_fn_s, "does not exist"
#             exit(2)
#         shutil.copy2(ds_fn_s, ds_fn)

#         #sort the data set
#         for heap in heaps:
#             log_fn = cases[case] + "_" + s + "_" + heap + "_log.txt"
#             log_f = open(log_fn, 'a+')
#             for i in range(iters):
#                 log_f.write("Begin_data_set " + str(idx) + " iter " + str(i) + 
#                             "\n")
#                 log_f.flush()
#                 subprocess.check_call(["/usr/bin/time -v " +  
#                                        "./heapsort -f " + ds_fn +  
#                                        " -t " + str(heaps[heap]) + " -d"], 
#                                       stdout = log_f, stderr = log_f, 
#                                       shell = True)
#                 log_f.flush()
#             log_f.close()

# # data sets with reversely sorted numbers
# case = 3
# for s in data_set_sizes:
#     length = convert(s)
#     for idx in range(sets_per_size):
#         #generate data set file
#         ds_fn_s = cases[0] + "_" + s + "_" + str(idx) + ".txt.out"
#         ds_fn = cases[case] + "_" + s + "_" + str(idx) + ".txt"
#         if not os.path.isfile(ds_fn_s):
#             print " Sorted file", ds_fn_s, "does not exist"
#             exit(2)
#         # reverse the file except the first line
#         ds_f = open(ds_fn, 'a+')
#         ds_s_f = open(ds_fn_s, 'r')
#         ds_f.write(ds_s_f.readline())
#         for line in reversed(ds_s_f.readlines()):
#             ds_f.write(line)
#         ds_f.flush()
#         ds_s_f.close()
#         ds_f.close()
#         #sort the data set
#         for heap in heaps:
#             log_fn = cases[case] + "_" + s + "_" + heap + "_log.txt"
#             log_f = open(log_fn, 'a+')
#             for i in range(iters):
#                 log_f.write("Begin_data_set " + str(idx) + " iter " + str(i) + 
#                             "\n")
#                 log_f.flush()
#                 subprocess.check_call(["/usr/bin/time -v " +  
#                                        "./heapsort -f " + ds_fn +  
#                                        " -t " + str(heaps[heap]) + " -d"], 
#                                       stdout = log_f, stderr = log_f, 
#                                       shell = True)
#                 log_f.flush()
#             log_f.close()
