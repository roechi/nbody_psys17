#!/usr/bin/python
import os
import time
import sys
import math

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
path_to_simulation_exe = "../cmake-build-debug/nbody_psys17"
path_to_plot_exe = "./plot_benchmark.py"

if not os.path.isfile(path_to_body_generation):
    print "{} does not exist!".format(path_to_body_generation)
    sys.exit();

if not os.path.isfile(path_to_simulation_exe):
    print "{} does not exist!".format(path_to_simulation_exe)
    sys.exit();

if not os.path.isfile(path_to_plot_exe):
    print "{} does not exist!".format(path_to_plot_exe)
    sys.exit();

print 'Benchmarking OMP vs. OpenCL vs. Sequential implementation.\n'

tmp_file="tmp.txt"
out_file="tmp_out.txt"
log_file='benchmark_nbody_{}.log'.format(current_milli_time())

num_bodies = [pow(2,x) for x in range(int(math.log(max_bodies,2))+1)]

f = open(log_file, 'w');

for cur_num in num_bodies:
    seq_times=[]
    omp_times=[]
    ocl_times=[]

    generate_bodies_call='{} {} {}'.format(path_to_body_generation, cur_num,tmp_file)
    os.system(generate_bodies_call)

    for i in range(0,num_repeats):
        # Benchmark sequential version (i.e. set number of threads to 1)
        simulation_call = 'OMP_NUM_THREADS={} {} {} {} {} {}'.format(1, path_to_simulation_exe, tmp_file, simulation_steps, 'omp', out_file)
        before = current_milli_time()
        os.system(simulation_call)
        seq_elapsed = current_milli_time() - before

        # Benchmark OMP simulation
        simulation_call = '{} {} {} {} {}'.format(path_to_simulation_exe, tmp_file, simulation_steps, 'omp', out_file)
        before = current_milli_time()
        os.system(simulation_call)
        omp_elapsed = current_milli_time() - before

        # Benchmark OpenCl simulation
        simulation_call = '{} {} {} {} {}'.format(path_to_simulation_exe, tmp_file, simulation_steps, 'ocl', out_file)
        before = current_milli_time()
        os.system(simulation_call)
        ocl_elapsed = current_milli_time() - before

        seq_times += [seq_elapsed]
        omp_times += [omp_elapsed]
        ocl_times += [ocl_elapsed]

    f.write('{};{};{};{}\n'.format(cur_num,seq_times,omp_times,ocl_times))
    f.flush()
    print 'Done. Simulation with {} bodies and {} steps finished, {} repeats.\nSequential: {}, OMP: {}, OpenCL: {}\n'.format(cur_num, simulation_steps, num_repeats, seq_times, omp_times, ocl_times)

f.close()
print 'Benchmarking done. Logfile saved in {}.\n'.format(log_file)

os.remove(tmp_file)
os.remove(out_file)
print 'Cleaned up temporary files.\n'

os.system('{} {}'.format(path_to_plot_exe, log_file))
