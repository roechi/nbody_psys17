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


class OmpSimulator : Simulator {

public:
    OmpSimulator(const std::string &input_file_path, const std::string &output_file_path, int simulation_steps);
    void startSimulation();

protected:

    void initializeBodies();
    void generateBodies();
    void scaleBodies();
    void loop();
    void addForces();

    std::list<Body> bodies;

private:


};


#include <fstream>

#endif /* SRC_OMPSIMULATOR_H_ */
