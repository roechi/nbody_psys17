/**
 * @file nbody.c
 *
 * This is the main driver of the program, i.e.,
 * the program, which is then used by the user.
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "body.h"
#include "core.h"

int main(int argc, char **argv)
{
	#pragma omp parallel
	{
		printf("Hello world from thread %d of %d!\n", omp_get_thread_num(), omp_get_num_threads());
	}

	const size_t N = 3;
	body nbodies[N];
	for (size_t i = 0; i < N; i++) {
		double x_pos = (double)rand()/(float)(RAND_MAX);
		double y_pos = (double)rand()/(float)(RAND_MAX);
		double x_vel = (double)rand()/(float)(RAND_MAX);
		double y_vel = (double)rand()/(float)(RAND_MAX);
		body b = init_body((double)10*(i+1), (double[2]){x_pos, y_pos}, (double[2]){x_vel, y_vel});
		nbodies[i] = b;
	}

	for (size_t i = 0; i < N; i++) {
		print_body(nbodies[i]);
	}

	return EXIT_SUCCESS;
}
