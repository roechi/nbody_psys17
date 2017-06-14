#!/usr/bin/python

import sys
import numpy
import matplotlib.pyplot as plot

if (len(sys.argv) != 2):
    print "Missing arguments.\nUsage: ./plot_benchmark.py [log_file]"
    sys.exit()

with open(sys.argv[1]) as file:
    content = file.readlines()
content = [line.strip() for line in content]

omp_runtimes = []
num_omp_threads = []

for line in content:
    values = line.split(';')
    num_omp_threads += [int(values[0])]
    omp_runtimes += [int(values[1])]

fig = plot.figure(figsize=(14, 7))

max_threads = len(num_omp_threads)

ax = plot.subplot()
ax.set_xlim(num_omp_threads[0], num_omp_threads[max_threads - 1])
line1 = ax.plot(num_omp_threads, omp_runtimes, label = 'OMP')
plot.xlabel('number of OMP threads')
plot.ylabel('simulation speed (ms)')
plot.title('OMP speedup for increased number of threads')

# Render, Control + C in command line to close the window
plot.draw()
plot.waitforbuttonpress(0)
plot.close(fig)
