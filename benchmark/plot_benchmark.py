#!/usr/local/bin/python
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

seq_times = []
omp_times=[]
ocl_times=[]

seq_stdevs = []
omp_stdevs=[]
ocl_stdevs=[]

num_bodies=[]

for line in content:
    vals = line.split(';')
    num_bodies += [int(vals[0])]

    seq_times += [numpy.mean(eval(vals[1]))]
    omp_times += [numpy.mean(eval(vals[2]))]
    ocl_times += [numpy.mean(eval(vals[3]))]

    seq_stdevs += [numpy.std(eval(vals[1]))]
    omp_stdevs += [numpy.std(eval(vals[2]))]
    ocl_stdevs += [numpy.std(eval(vals[3]))]

    if int(vals[0]) == 2048:
        print "Seq mean={}, Seq std={}".format(numpy.mean(eval(vals[1])), numpy.std(eval(vals[1])))
        print "OMP mean={}, OMP std={}, Speedup={}".format(numpy.mean(eval(vals[2])), numpy.std(eval(vals[2])), numpy.mean(eval(vals[1])) / numpy.mean(eval(vals[2])))
        print "OCL mean={}, OCL std={}, Speedup={}".format(numpy.mean(eval(vals[3])), numpy.std(eval(vals[3])), numpy.mean(eval(vals[1])) / numpy.mean(eval(vals[3])))



N=len(content)
ind = [x for x in numpy.arange(N)]
# ind = num_bodies
fig = plt.figure(figsize=(14, 7))
ax = plt.subplot()
line1 = ax.errorbar(ind, seq_times, yerr=seq_stdevs, label = 'Sequential')
line2 = ax.errorbar(ind, omp_times, yerr=omp_stdevs, label = 'OMP')
line3 = ax.errorbar(ind, ocl_times, yerr=ocl_stdevs, label = 'OpenCL')

ax.legend(loc='upper left')
# ax.set_xlim([0, N+1])
plt.xticks(ind, num_bodies)
plt.xlabel('Number of bodies')
plt.ylabel('Simulation speed (ms)')
plt.title('Sequential vs. OMP vs. OpenCL simulation speed')

# Render, Control + C in command line to close the window
plt.draw()
plt.waitforbuttonpress(0)
plt.close(fig)
