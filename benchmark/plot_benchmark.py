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

omp_times=[]
ocl_times=[]
num_bodies=[]

for line in content:
    vals = line.split(';')
    num_bodies += [int(vals[0])]
    omp_times += [int(vals[1])]
    ocl_times += [int(vals[2])]


N=len(content)
ind = [x for x in numpy.arange(N)]
# ind = num_bodies
fig = plt.figure(figsize=(14, 7))
ax = plt.subplot()
line1 = ax.plot(ind, omp_times, label = 'OMP')
line2 = ax.plot(ind, ocl_times, label = 'OpenCL')
ax.legend(loc='upper left')
# ax.set_xlim([0, N+1])
plt.xticks(ind, num_bodies)
plt.xlabel('Number of bodies')
plt.ylabel('Simulation speed (ms)')
plt.title('OMP vs OpenCL simulation speed')

# Render, Control + C in command line to close the window
plt.draw()
plt.waitforbuttonpress(0)
plt.close(fig)
