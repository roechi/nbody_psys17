#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "Body.h"
#include "Simulator.h"
#include <utility>

int main(int argc, char **argv)
{
	#pragma omp parallel
	{
		fprintf(stderr,"Hello world from thread %d of %d!\n", omp_get_thread_num(), omp_get_num_threads());
	}

	int num_bodies = 0;
	int simulation_steps = 0;
	std::string file_path = "";

	if(argc < 3) {
		fprintf(stderr, "Provide NUM_BODIES, SIMULATION_STEPS and OUT_FILE as arguments!\n");
		return EXIT_FAILURE;
	} else {
		num_bodies = atoi(argv[1]);
		simulation_steps = atoi(argv[2]);
		file_path = std::string(argv[3]);
	}

	fprintf(stderr, "Starting simulation with %d bodies and %d simulation steps. Writing to file %s\n", num_bodies, simulation_steps, file_path.c_str());
	Simulator simulator = Simulator(file_path);
	simulator.startSimulation(num_bodies, simulation_steps);

	return EXIT_SUCCESS;
}
