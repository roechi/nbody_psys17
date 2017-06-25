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
omp_stdevs = []
num_omp_threads = []
num_bodies = 0
simulation_steps = 0

line_idx = 0
for line in content:
    if line_idx == 0:
        num_bodies = int(line)
    elif line_idx == 1:
        simulation_steps = int(line)
    elif line_idx == 2:
        repeats = int(line)
    else:
        values = line.split(';')
        num_omp_threads += [int(values[0])]
        omp_runtimes += [numpy.mean(eval(values[1]))]
        omp_stdevs += [numpy.std(eval(values[1]))]
    line_idx = line_idx + 1

fig = plot.figure(figsize=(14, 7))

max_threads = len(num_omp_threads)

ax = plot.subplot()
ax.set_xlim(num_omp_threads[0], num_omp_threads[max_threads - 1])
line1 = ax.errorbar(num_omp_threads, omp_runtimes, yerr=omp_stdevs, label = 'OMP')
plot.xlabel('number of OMP threads')
plot.ylabel('simulation speed (ms)')
plot.title('OpenMP simulation speed with {} bodies and {} steps for {} runs'.format(num_bodies, simulation_steps, repeats))

# Render, Control + C in command line to close the window
plot.draw()
plot.waitforbuttonpress(0)
plot.close(fig)
