#!/usr/bin/env python

"""
    This script requires python-graph package from
    https://code.google.com/p/python-graph/
"""


import random
import sys
import pygraph.algorithms.generators as generator

if (len(sys.argv) != 3):
    print "./%s num_vertex num_edge" % (sys.argv[0])
    sys.exit()

num_vertex = int(sys.argv[1])
num_edge = int(sys.argv[2])

g = generator.generate(num_vertex, num_edge,
                       directed=False, weight_range=(1,100))

edge_list = g.edges()

with open("sample.graph", 'wb') as f:
    f.write("%d\n"%num_vertex)
    for x,y in edge_list:
        w = random.random() * 100
        f.write("%d %d %f\n" % (x, y, w))
