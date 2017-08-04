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

write_times = []
no_write_times=[]

write_stdevs = []
no_write_stdevs=[]

num_bodies=[]

for line in content:
    vals = line.split(';')
    num_bodies += [int(vals[0])]

    m1 = numpy.mean(eval(vals[1]))
    m2 = numpy.mean(eval(vals[2]))
    # write_times += [m1]
    # no_write_times += [m2]

    s1 = numpy.std(eval(vals[1]))
    s2 = numpy.std(eval(vals[2]))
    # write_stdevs += [s1]
    # no_write_stdevs += [s2]

    # p = (m2 - m1) / m2
    # seq = 1 - p
    # print p, seq
    print m1,m2
    par = (m1 - m2) / m1
    seq = 1 - par
    # if par > 0 and seq > 0:
    write_times += [par]
    no_write_times += [seq]

ps = write_times
print ps

N=len(write_times)
ind = [x for x in numpy.arange(N)]
# ind = num_bodies
fig = plt.figure(figsize=(14, 7))
ax = plt.subplot()
line1 = plt.bar(ind, write_times, width=0.5, label = 'Parallel part')
line2 = plt.bar(ind, no_write_times, bottom=write_times,width=0.5, label = 'Sequential part')
# line3 = plt.plot(ind, write_times, color='black')

ax.legend(loc='upper left')
# ax.set_xlim([0, N+1])
plt.xticks(ind, num_bodies)
plt.xlabel('Number of bodies')
plt.ylabel('Share of execution time')
plt.title('Sequential and parallel parts')

# Render, Control + C in command line to close the window
plt.draw()
plt.waitforbuttonpress(0)
plt.close(fig)
