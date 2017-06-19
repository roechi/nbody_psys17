#!/usr/bin/python
import os
import time

current_millies_time = lambda: int(round(time.time() * 1000))

path_to_body_generation = "../resources/generateRandomGalaxy.py"
path_to_simulation_exe = "../cmake-build-debug/nbody_psys17"

tmp_file = "tmp_omp.txt"
out_file = "tmp_omp_out.txt"
log_file = "benchmark_nbody_{}.log".format(current_millies_time())

num_bodies = 100
simulation_steps = 1000
num_threads = 4
num_repeats = 8

results_file = open(log_file, 'w')

generate_bodies_call = '{} {} {}'.format(
    path_to_body_generation,
    num_bodies,
    tmp_file
)
os.system(generate_bodies_call)

for curr_num_threads in range(1, 17):
    omp_elapsed = []
    simulation_call = 'OMP_NUM_THREADS={} {} {} {} {} {}'.format(
        curr_num_threads,
        path_to_simulation_exe,
        tmp_file,
        simulation_steps,
        'omp',
        out_file
    )

    print "built call:\n" + simulation_call + "\n"

    for i in range(num_repeats):
        before = current_millies_time()
        os.system(simulation_call)
        omp_elapsed += [current_millies_time() - before]

    results_file.write('{};{}\n'.format(curr_num_threads, omp_elapsed))
    results_file.flush()
    print 'Done. Omp simulation with {} threads and {} steps finished in {} milliseconds'.format(
        curr_num_threads, simulation_steps, omp_elapsed
    )

results_file.close()

print 'Benchmarking done. Logfile saved to {}.\n'.format(log_file)
os.remove(tmp_file)
os.remove(out_file)
print 'All cleaned up.\n'

os.system('./plot_omp_benchmark.py {}'.format(log_file))
