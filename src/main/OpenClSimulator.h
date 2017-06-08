//
// Created by Jonas Jaszkowic on 08.06.17.
//

#ifndef NBODY_PSYS17_OPENCLSIMULATOR_H
#define NBODY_PSYS17_OPENCLSIMULATOR_H

#include "Simulator.h"

class OpenClSimulator : public Simulator {

public:
    OpenClSimulator(const std::string &input_file_path, const std::string &output_file_path, int simulation_steps);
    void startSimulation();
    void initializeBodies();
    void scaleBodies();

private:

    /**
     * These members represent our bodies
     */
    float* masses;
    float* positions;
    float* velocities;
    float* forces;

};


#endif //NBODY_PSYS17_OPENCLSIMULATOR_H
