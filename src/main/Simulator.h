/*
 * Simulator.h
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#ifndef SRC_SIMULATOR_H_
#define SRC_SIMULATOR_H_
#include "Body.h"
#include "ConfigParser.h"
#include <iostream>
#include <fstream>

class Simulator {
public:
	Simulator(std::string input_file_path);
	Simulator(std::string input_file_path, std::string output_file_path);
	int startSimulation();
	int startSimulation(int simulation_steps);

	int NUM_BODIES = 5;
	int SIMULATION_STEPS = 10000;

//	static constexpr double RADIUS_UNIVERSE = 300000000e3;
//	static constexpr double SOLAR_MASS = 1.98892e30;
//	static constexpr double GRAVITATIONAL_CONSTANT = 6.67e-11;
//	static constexpr double UPDATE_STEP = 86400; // in seconds = 1 astronomical day

// Time is measured in years, distances in AU and masses in solar-masses

    static constexpr double RADIUS_UNIVERSE = 3.0;
    static constexpr double SOLAR_MASS = 1.0;
    static constexpr double GRAVITATIONAL_CONSTANT = 39.5;
    static constexpr double UPDATE_STEP = 1.0 / 365.25; // Update step 1 day
    static constexpr double MASS_SCALE = 1.98892e+30; // 1 Solar Mass in kg
    static constexpr double LENGTH_SCALE =  149597870700; // 1 AU in m
	static constexpr double TIME_SCALE = 86400 * 365.25; // 1 year in seconds

private:

	enum class Mode {FILE, CONSOLE};

	Mode mode;
	Body* bodies;
    std::string input_file_path;

	void generateBodies();
	void addForces();
	int loop();
	void scaleBodies();

	double getRandom();
	double circularVelocity(double rx, double ry);
	double signum(double val);
	double exp(double lambda);

    std::ofstream file;

	void runStep();
};

#endif /* SRC_SIMULATOR_H_ */
