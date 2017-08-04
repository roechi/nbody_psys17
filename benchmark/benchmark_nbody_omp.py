#!/usr/bin/python
import os
import time
import sys

current_millies_time = lambda: int(round(time.time() * 1000))

if (len(sys.argv) != 4):
    print "Missing arguments.\nUsage: {} [num_bodies] [simulation_steps] [repeats]".format(sys.argv[0])
    sys.exit()

num_bodies = int(sys.argv[1])
simulation_steps = int(sys.argv[2])
num_repeats = int(sys.argv[3])

path_to_body_generation = "../resources/generateRandomGalaxy.py"
path_to_simulation_exe = "../cmake-build-debug/nbody_psys17"
path_to_plot_exe = "./plot_omp_benchmark.py"

if not os.path.isfile(path_to_body_generation):
    print "{} does not exist!".format(path_to_body_generation);
    sys.exit();

if not os.path.isfile(path_to_simulation_exe):
    print "{} does not exist!".format(path_to_simulation_exe);
    sys.exit();

if not os.path.isfile(path_to_plot_exe):
    print "{} does not exist!".format(path_to_plot_exe);
    sys.exit();

tmp_file = "tmp_omp.txt"
out_file = "tmp_omp_out.txt"
log_file = "benchmark_nbody_{}.log".format(current_millies_time())

results_file = open(log_file, 'w')
results_file.write('{}\n{}\n{}\n'.format(num_bodies,simulation_steps,num_repeats))

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

os.system('{} {}'.format(path_to_plot_exe, log_file))
