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
#include "Mode.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <fstream>

class Simulator {

public:

    /**
     * Default constructor for the Simulator
     * @param input_file_path The path to the config file
     * @param output_file_path The path to the output file where the positions are stored
     */
    Simulator(const std::string& input_file_path, const std::string& output_file_path, int simulation_steps) {
        this->input_file_path = input_file_path;
        this->simulation_steps = simulation_steps;
        this->output_file.open(output_file_path);
        this->mode = Mode::FILE;
    }

    Simulator(const std::string& input_file_path, int simulation_steps) {
        this->input_file_path = input_file_path;
        this->simulation_steps = simulation_steps;
        this->mode = Mode::CONSOLE;
    }

    /**
     * Starts the simulation and performs simulation_steps steps, which is set in the constructor.
     * Don't forget to close() the output file when the simulation finished!
     */
    virtual int startSimulation() = 0;


    /**
     * Constants for the simulation:
     * Time is measured in years, distances in AU and masses in solar-masses
     * This allows us to use float instead of float because the values will most likely stay in the float scale
     * This is needed because OpenCL kernels do not support float values (depending on hardware)
     */
    static constexpr double RADIUS_UNIVERSE = 3.0;
    static constexpr double SOLAR_MASS = 1.0;
    static constexpr double GRAVITATIONAL_CONSTANT = 39.5;
    static constexpr double UPDATE_STEP = 1.0 / 365.25; // Update step 1 day
    static constexpr double MASS_SCALE = 1.98892e+30; // 1 Solar Mass in kg
    static constexpr double LENGTH_SCALE =  149597870700; // 1 AU in m
    static constexpr double TIME_SCALE = 86400 * 365.25; // 1 year in seconds

protected:

    /**
     * Initializes the bodies, subclasses have to decide how each Body is represented
     * and how to parse the config file with the help of ConfigParser
     */
    virtual void initializeBodies() = 0;

    /**
     * Total number of bodies participating in the simulation, this value is set
     * after parsing the config file and determining the number of bodies
     */
    int num_bodies = -1;

    /**
     * Number of simulation steps, execution stops after this number of steps. This value
     * should be set according to the constructor parameter
     */
    int simulation_steps = -1;

    std::string input_file_path;
    std::ofstream output_file;

    double getRandom() {
        double r = ((double) rand() / (RAND_MAX));
        return r;
    }

    double circularVelocity(double rx, double ry) {
        double r2=sqrt(rx*rx+ry*ry);
        double numerator=(GRAVITATIONAL_CONSTANT)*1e6*SOLAR_MASS;
        return sqrt(numerator/r2);
    }

    double signum(double val) {
        return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
    }

    double exp(double lambda) {
        return -log(1 - getRandom()) / lambda;
    }

    Mode mode;
};

#endif /* SRC_SIMULATOR_H_ */
