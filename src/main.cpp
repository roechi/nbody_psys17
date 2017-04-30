#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "Body.h"
#include "Simulator.h"
#include <utility>

int main(int argc, char **argv)
{
//	#pragma omp parallel
//	{
//		printf("Hello world from thread %d of %d!\n", omp_get_thread_num(), omp_get_num_threads());
//	}

	Simulator simulator = Simulator();
	simulator.startSimulation();

	return EXIT_SUCCESS;
}
