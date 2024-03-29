#!/usr/bin/python
import os
import time
import sys
import math

current_milli_time = lambda: int(round(time.time() * 1000))

if (len(sys.argv) != 4):
    print "Missing arguments.\nUsage: ./benchmark_nbody_ocl.py [num_bodies] [simulation_steps] [repeats]"
    sys.exit()

num_bodies = int(sys.argv[1])
simulation_steps = int(sys.argv[2])
num_repeats = int(sys.argv[3])

if not (num_bodies != 0 and ((num_bodies & (num_bodies - 1)) == 0)):
    print "{} is not a power of 2. Please provide num_bodies that is a power of 2.".format(num_bodies)
    sys.exit()

path_to_body_generation = "../resources/generateRandomGalaxy.py"
path_to_simulation_exe = "../cmake-build-debug/nbody_psys17"
path_to_plot_exe = "./plot_ocl_benchmark.py"

if not os.path.isfile(path_to_body_generation):
    print "{} does not exist!".format(path_to_body_generation)
    sys.exit();

if not os.path.isfile(path_to_simulation_exe):
    print "{} does not exist!".format(path_to_simulation_exe);
    sys.exit();

if not os.path.isfile(path_to_plot_exe):
    print "{} does not exist!".format(path_to_plot_exe);
    sys.exit();

print 'Benchmarking OpenCL implementation with varying work group sizes.\n'

tmp_file="tmp.txt"
out_file="tmp_out.txt"
log_file='benchmark_nbody_{}.log'.format(current_milli_time())
work_group_sizes = [pow(2,x) for x in range(int(math.log(num_bodies,2))+1)]

f = open(log_file, 'w');
f.write('{}\n{}\n{}\n'.format(num_bodies,simulation_steps,num_repeats))

generate_bodies_call='{} {} {}'.format(path_to_body_generation, num_bodies,tmp_file)
os.system(generate_bodies_call)

for work_group_size in work_group_sizes:
    ocl_times=[]

    print '\nBenchmarking for work_group_size={}\n'.format(work_group_size)
    for i in range(0,num_repeats):
        simulation_call = '{} {} {} {} {} {}'.format(path_to_simulation_exe, tmp_file, simulation_steps, 'ocl', out_file, work_group_size)
        before = current_milli_time()
        os.system(simulation_call)
        ocl_elapsed = current_milli_time() - before
        ocl_times += [ocl_elapsed]

    f.write('{};{}\n'.format(work_group_size,ocl_times))
    f.flush()
    print 'Done. Simulation with {} bodies and {} steps and {} finished, {} repeats.\nOpenCL: {}\n'.format(num_bodies, simulation_steps, work_group_size, num_repeats, ocl_times)

f.close()
print 'Benchmarking done. Logfile saved in {}.\n'.format(log_file)

os.remove(tmp_file)
os.remove(out_file)
print 'Cleaned up temporary files.\n'

os.system('{} {}'.format(path_to_plot_exe,log_file))
