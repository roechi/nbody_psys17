/**
 * @file nbody.c
 *
 * This is the main driver of the program, i.e.,
 * the program, which is then used by the user.
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv)
{
	
	#pragma omp parallel
	{
		printf("Hello world from thread %d of %d!\n", omp_get_thread_num(), omp_get_num_threads());
	}

	return EXIT_SUCCESS;
}
