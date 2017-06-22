#!/usr/bin/python
import os
import sys
import subprocess
import numpy
import matplotlib.pyplot as plt

if (len(sys.argv) != 2):
    print "Missing arguments.\nUsage: ./plot_benchmark.py [log_file]"
    sys.exit()

with open(sys.argv[1]) as f:
    content = f.readlines()
content = [x.strip() for x in content]

ocl_times=[]
ocl_stdevs=[]
num_threads=[]
num_bodies = 0
simulation_steps = 0

line_idx = 0
for line in content:
    if line_idx == 0:
        num_bodies = int(line)
    elif line_idx == 1:
        simulation_steps = int(line)
    else:
        vals = line.split(';')
        num_threads += [int(vals[0])]
        ocl_times += [numpy.mean(eval(vals[1]))]
        ocl_stdevs += [numpy.std(eval(vals[1]))]
    line_idx = line_idx + 1



N=len(content)-2
ind = [x for x in numpy.arange(N)]
# ind = num_threads
fig = plt.figure(figsize=(14, 7))
ax = plt.subplot()
line2 = ax.errorbar(ind, ocl_times, yerr=ocl_stdevs, label = 'OpenCL')
ax.legend(loc='upper left')
# ax.set_xlim([0, N+1])
plt.xticks(ind, num_threads)
plt.xlabel('Work group size')
plt.ylabel('Simulation speed (ms)')
plt.title('OpenCL simulation speed with {} bodies and {} steps'.format(num_bodies, simulation_steps))

# Render, Control + C in command line to close the window
plt.draw()
plt.waitforbuttonpress(0)
plt.close(fig)
