/*
 * Simulator.h
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#ifndef SRC_SIMULATOR_H_
#define SRC_SIMULATOR_H_
#include "Body.h"

class Simulator {
public:
	Simulator();
	void startSimulation();

	static constexpr int NUM_BODIES = 12;
	static constexpr int SIMULATION_STEPS = 10000;
	static constexpr double RADIUS_UNIVERSE = 1e18;
	static constexpr double SOLAR_MASS = 1.98892e30;
	static constexpr double GRAVITATIONAL_CONSTANT = 6.67e-11;
	static constexpr double UPDATE_STEP = 1.5e11;

private:

	Body* bodies;

	void generateBodies();
	void addForces();
	void loop();

	double getRandom();
	double circlev(double rx, double ry);
	double signum(double val);
	double exp(double lambda);
};

#endif /* SRC_SIMULATOR_H_ */
