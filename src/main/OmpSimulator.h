/*
 * Simulator.h
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#ifndef SRC_OMPSIMULATOR_H_
#define SRC_OMPSIMULATOR_H_
#include "Body.h"
#include "ConfigParser.h"
#include "Simulator.h"
#include <iostream>


class OmpSimulator : public Simulator {

public:
    OmpSimulator(const std::string &input_file_path, const std::string &output_file_path);
    OmpSimulator(const std::string &input_file_path);
    int startSimulation(int simulation_steps);
    void initializeBodies();

protected:

    void generateBodies();
    void scaleBodies();
    int loop(int i);
    void addForces();

    Body* bodies;

private:
    void runStep();
};


#include <fstream>

#endif /* SRC_OMPSIMULATOR_H_ */
