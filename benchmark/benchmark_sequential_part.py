#!/usr/bin/python
import os
import time
import sys
import math
import numpy

# Benchmarks the OpenCL and OMP implementation
current_milli_time = lambda: int(round(time.time() * 1000))

if (len(sys.argv) != 4):
    print "Missing arguments.\nUsage: {} [max_bodies] [simulation_steps] [repeats]".format(sys.argv[0])
    sys.exit()

max_bodies = int(sys.argv[1])
simulation_steps = int(sys.argv[2])
num_repeats = int(sys.argv[3])

if not (max_bodies != 0 and ((max_bodies & (max_bodies - 1)) == 0)):
    print "{} is not a power of 2. Please provide num_bodies that is a power of 2.".format(num_bodies)
    sys.exit()

path_to_body_generation = "../resources/generateRandomGalaxy.py"
path_to_simulation_exe = "../cmake-build-debug/nbody_psys17_default"
path_to_modified_exe = "../cmake-build-debug/nbody_psys17_mod"

if not os.path.isfile(path_to_body_generation):
    print "{} does not exist!".format(path_to_body_generation)
    sys.exit();

if not os.path.isfile(path_to_simulation_exe):
    print "{} does not exist!".format(path_to_simulation_exe)
    sys.exit();

if not os.path.isfile(path_to_modified_exe):
    print "{} does not exist!".format(path_to_modified_exe)
    sys.exit();

print 'Benchmarking sequential part.\n'

tmp_file="tmp.txt"
out_file="tmp_out.txt"
log_file='benchmark_nbody_{}.log'.format(current_milli_time())

f = open(log_file, 'w');
num_bodies = [pow(2,x) for x in range(5, int(math.log(max_bodies,2))+1)]
for cur_num in num_bodies:
    write_times=[]
    no_write_times=[]
    generate_bodies_call='{} {} {}'.format(path_to_body_generation, cur_num,tmp_file)
    os.system(generate_bodies_call)
    for i in range(0,num_repeats):
        # Run with write to file
        simulation_call = '{} {} {} {} {}'.format(path_to_simulation_exe, tmp_file, simulation_steps, 'omp', out_file)
        before = current_milli_time()
        os.system(simulation_call)
        write_elapsed = current_milli_time() - before

        # Run without write to file
        simulation_call = '{} {} {} {} {}'.format(path_to_modified_exe, tmp_file, simulation_steps, 'omp', out_file)
        before = current_milli_time()
        os.system(simulation_call)
        no_write_elapsed = current_milli_time() - before

        write_times += [write_elapsed]
        no_write_times += [no_write_elapsed]

    f.write('{};{};{}\n'.format(cur_num,write_times,no_write_times))
    f.flush()
    print 'Done. Simulation with {} bodies and {} steps finished, {} repeats.\nWith calculation: {}, Without calculation: {}\n'.format(num_bodies, simulation_steps, num_repeats, write_times, no_write_times)
    m1 = numpy.mean(write_times)
    m2 = numpy.mean(no_write_times)
    print "{} vs. {} -> Parallel share: {}\n".format(m1, m2, (m1-m2)/m1)

f.close()
print 'Benchmarking done. Logfile saved in {}.\n'.format(log_file)

os.system("./plot_sequential_part.py {}".format(log_file))

os.remove(tmp_file)
os.remove(out_file)
print 'Cleaned up temporary files.\n'
