#!/usr/bin/python
import os
import time

# Benchmarks the OpenCL and OMP implementation
# TODO: Check if scripts for generating bodies and executable is present
# TODO: Build project when executable is not present
# TODO: Allow script to be exited with CTRL-C (not working because of os.system subcalls)
# TODO: Set maximum number of body via parameter

current_milli_time = lambda: int(round(time.time() * 1000))

path_to_body_generation = "../resources/generateRandomGalaxy.py"
path_to_simulation_exe = "../cmake-build-debug/nbody_psys17"

print 'Benchmarking OMP vs. OpenCL implementation.\n'

tmp_file="tmp.txt"
out_file="tmp_out.txt"
log_file='benchmark_nbody_{}.log'.format(current_milli_time())
omp_times=[]
ocl_times=[]

num_bodies = [pow(2,x) for x in range(1,11)]
simulation_steps = 1000;

f = open(log_file, 'w');

for cur_num in num_bodies:
    generate_bodies_call='{} {} {}'.format(path_to_body_generation, cur_num,tmp_file)
    os.system(generate_bodies_call)

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

    omp_times += [omp_elapsed]
    ocl_times += [ocl_elapsed]
    f.write('{};{};{}\n'.format(cur_num,omp_elapsed,ocl_elapsed))
    f.flush()
    print 'Done. Simulation with {} bodies and {} steps finished.\nOMP: {}, OpenCL: {}\n'.format(cur_num, simulation_steps, omp_elapsed, ocl_elapsed)

f.close()
print 'Benchmarking done. Logfile saved in {}.\n'.format(log_file)

os.remove(tmp_file)
os.remove(out_file)
print 'Cleaned up temporary files.\n'

os.system('./plot_benchmark.py {}'.format(log_file))
