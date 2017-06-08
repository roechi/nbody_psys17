//
// Created by Jonas Jaszkowic on 08.06.17.
//

#include "OpenClSimulator.h"

OpenClSimulator::OpenClSimulator(const std::string &input_file_path, const std::string &output_file_path, int simulation_steps)
        : Simulator(input_file_path, output_file_path, simulation_steps) {
    initializeBodies();
}

void OpenClSimulator::initializeBodies() {
    
}

void OpenClSimulator::startSimulation() {

}

