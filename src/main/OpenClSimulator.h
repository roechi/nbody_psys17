//
// Created by Jonas Jaszkowic on 08.06.17.
//

#ifndef NBODY_PSYS17_OPENCLSIMULATOR_H
#define NBODY_PSYS17_OPENCLSIMULATOR_H

#include <cl.hpp>
#include "Simulator.h"

class OpenClSimulator : public Simulator {

public:
    OpenClSimulator(const std::string &input_file_path, const std::string &output_file_path);
    OpenClSimulator(const std::string &input_file_path);
    int startSimulation(int simulation_steps);
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

    void runStep(cl::Buffer &buffer_masses, cl::Buffer &buffer_positions, cl::Buffer &buffer_velocities,
                 cl::CommandQueue &queue, cl::Kernel &kernel_add);
};


#endif //NBODY_PSYS17_OPENCLSIMULATOR_H
