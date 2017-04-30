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

private:
	static constexpr int NUM_BODIES = 100;
	static constexpr int SIMULATION_STEPS = 1000;

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
