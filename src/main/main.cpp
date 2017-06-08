#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "Body.h"
#include "OmpSimulator.h"
#include <utility>

int main(int argc, char **argv)
{
	#pragma omp parallel
	{
		fprintf(stderr,"Hello world from thread %d of %d!\n", omp_get_thread_num(), omp_get_num_threads());
	}

	std::string input_file_path = "";
	int simulation_steps = 0;
	std::string output_file_path = "";
	int result = 0;

	if (argc == 3) {
		input_file_path = std::string(argv[1]);
		simulation_steps = atoi(argv[2]);

		fprintf(stderr, "Starting simulation from file %s with %d simulation steps. Writing to console...\n", input_file_path.c_str(), simulation_steps, output_file_path.c_str());
		Simulator simulator = Simulator(input_file_path);
		result = simulator.startSimulation(simulation_steps);
	} else if(argc == 4) {
		input_file_path = std::string(argv[1]);
		simulation_steps = atoi(argv[2]);
		output_file_path = std::string(argv[3]);

		fprintf(stderr, "Starting simulation from file %s with %d simulation steps. Writing to file %s\n", input_file_path.c_str(), simulation_steps, output_file_path.c_str());
		Simulator simulator = Simulator(input_file_path, output_file_path);
		result = simulator.startSimulation(simulation_steps);
	} else {
		fprintf(stderr, "Provide IN_FILE, SIMULATION_STEPS (and optionally OUT_FILE) as arguments!\n");
		return EXIT_FAILURE;
	}

	return result;
}
