#!/usr/local/bin/python
import os
import sys
import subprocess
import numpy
import matplotlib.pyplot as plt

ahmdal = lambda p, s: 1 / ((1 - p) + (p/s))

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

num_processors=[pow(2,x) for x in range(1,17)]

N=len(num_processors)
ind = [x for x in numpy.arange(N)]

fig = plt.figure(figsize=(14, 7))
ax = plt.subplot()

for p in ps:
    l = plt.plot(ind,[ahmdal(p,x) for x in num_processors],label="n={}".format(num_bodies[ps.index(p)]))

#
# line1 = plt.bar(ind, write_times, width=0.5, label = 'Parallel part')
# line2 = plt.bar(ind, no_write_times, bottom=write_times,width=0.5, label = 'Sequential part')


ax.legend(loc='upper left')
# ax.set_xlim([0, N+1])
plt.xticks(ind, num_processors)
plt.xlabel('Number of processors')
plt.ylabel('Theoretical speedup')
plt.title('Theoretical speedup for varying number of bodies and processors')

# Render, Control + C in command line to close the window
plt.draw()
plt.waitforbuttonpress(0)
plt.close(fig)
