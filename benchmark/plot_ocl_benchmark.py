#!/usr/bin/python

import sys
import numpy
import matplotlib.pyplot as plot
import string

if (len(sys.argv) != 2):
    print "Missing arguments.\nUsage: ./plot_benchmark.py [log_file]"
    sys.exit()

with open(sys.argv[1]) as file:
    content = file.readlines()
content = [line.strip() for line in content]

ocl_runtimes = []
ocl_stdevs = []
ocl_wgroup_size = []

for line in content:
    values = line.split(';')
    ocl_wgroup_size += [int(values[0])]
    ocl_runtimes += [numpy.mean(eval(values[1]))]
    ocl_stdevs += [numpy.std(eval(values[1]))]

fig = plot.figure(figsize=(14, 7))

max_threads = len(ocl_wgroup_size)

ax = plot.subplot()
ax.set_xlim(ocl_wgroup_size[0], ocl_wgroup_size[max_threads - 1])
line1 = ax.errorbar(ocl_wgroup_size, ocl_runtimes, yerr=ocl_stdevs, label ='OMP')
plot.xlabel('size of OCL work group')
plot.ylabel('simulation speed (ms)')
plot.title('OCL speedup for increased work group size')

# Render, Control + C in command line to close the window
plot.draw()
plot.waitforbuttonpress(0)
plot.close(fig)
