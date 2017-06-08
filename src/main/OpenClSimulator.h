//
// Created by Jonas Jaszkowic on 08.06.17.
//

#ifndef NBODY_PSYS17_OPENCLSIMULATOR_H
#define NBODY_PSYS17_OPENCLSIMULATOR_H

#include "Simulator.h"

class OpenClSimulator : public Simulator {
    OpenClSimulator(const std::string &input_file_path, const std::string &output_file_path, int simulation_steps);
    void startSimulation();
    void initializeBodies();
};


#endif //NBODY_PSYS17_OPENCLSIMULATOR_H
