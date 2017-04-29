#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "Body.h"
#include <utility>

int main(int argc, char **argv)
{
	#pragma omp parallel
	{
		printf("Hello world from thread %d of %d!\n", omp_get_thread_num(), omp_get_num_threads());
	}

	Body* b = new Body(5, tuple(3,4), tuple(5,6));
	b->print();

	return EXIT_SUCCESS;
}
