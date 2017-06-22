#!/usr/bin/python
import os
import time

# Benchmarks the OpenCL implementation

current_milli_time = lambda: int(round(time.time() * 1000))

path_to_body_generation = "../resources/generateRandomGalaxy.py"
path_to_simulation_exe = "../cmake-build-debug/nbody_psys17"

print 'Benchmarking OpenCL implementation with varying work group sizes.\n'

tmp_file="tmp.txt"
out_file="tmp_out.txt"
log_file='benchmark_nbody_{}.log'.format(current_milli_time())
num_repeats = 8

work_group_sizes = [pow(2,x) for x in range(1,7)]
num_bodies = 128
simulation_steps = 1000;

f = open(log_file, 'w');

generate_bodies_call='{} {} {}'.format(path_to_body_generation, num_bodies,tmp_file)
os.system(generate_bodies_call)

for work_group_size in work_group_sizes:
    ocl_times=[]

    print '\nBenchmarking for work_group_size={}\n'.format(work_group_size)
    for i in range(0,num_repeats):
        # Benchmark OpenCl simulation
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

# os.system('./plot_benchmark.py {}'.format(log_file))
