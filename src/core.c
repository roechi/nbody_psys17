/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */

#include "core.h"
#include "body.h"
#include <math.h>
#include <stdlib.h>

double* get_acceleration(body* nbodies, size_t idx) {
	double *accel = malloc (sizeof (double) * 2);
	double grav = 6.67 * pow(10, -11);
	double eps = 0.01;
	body boi = nbodies[idx];
	size_t N = 3;
	double sum[2] = {0.0, 0.0};

	for (size_t i = 0; i < N; i++) {
		body b = nbodies[i];

		double r[2] = {boi.position[0] - b.position[0], boi.position[1] - b.position[1]};
		double weighted_r[2] = {b.mass * r[0], b.mass * r[1]};

		double dist_sq = get_distance(boi, b);
		double div = pow(dist_sq + (eps*eps), 3.0/2.0);

		double final_r[2] = {weighted_r[0] / div, weighted_r[1] / div};

		sum[0] = sum[0] + final_r[0];
		sum[1] = sum[1] + final_r[1];
	}

	accel[0] = sum[0] * grav;
	accel[1] = sum[1] * grav;
	return accel;
}
