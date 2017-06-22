#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "Body.h"
#include "OmpSimulator.h"
#include "OpenClSimulator.h"
#include <utility>

int main(int argc, char **argv) {
//#pragma omp parallel
//    {
//        fprintf(stderr, "Hello world from thread %d of %d!\n", omp_get_thread_num(), omp_get_num_threads());
//    }
    Simulator *simulator;

    int result = EXIT_SUCCESS;
    try {
        if (argc == 4 || argc == 5 || argc == 6) {
            std::string input_file_path = std::string(argv[1]);
            int simulation_steps = atoi(argv[2]);
            std::string simulator_selector = std::string(argv[3]);

            if (argc == 5) {
                std::string output_file_path = std::string(argv[4]);

                if (simulator_selector.compare("ocl") == 0) {
                    simulator = new OpenClSimulator(input_file_path, output_file_path);
                    fprintf(stderr,
                            "Starting simulation from file %s with %d simulation steps. Running with OpenCL. Writing to file %s\n",
                            input_file_path.c_str(), simulation_steps, output_file_path.c_str());
                } else if (simulator_selector.compare("omp") == 0) {
                    simulator = new OmpSimulator(input_file_path, output_file_path);
                    fprintf(stderr,
                            "Starting simulation from file %s with %d simulation steps. Running with OpenMP. Writing to file %s\n",
                            input_file_path.c_str(), simulation_steps, output_file_path.c_str());
                } else {
                    throw new std::runtime_error(std::string("Unknown simulator ") + simulator_selector + ".\n");
                }

                simulator->startSimulation(simulation_steps);
            } else if (argc == 6) {
                std::string output_file_path = std::string(argv[4]);
                int work_groups = atoi(argv[5]);
                if (simulator_selector.compare("ocl") == 0) {
                    simulator = new OpenClSimulator(input_file_path, output_file_path);
                    ((OpenClSimulator*)simulator)->setWorkGroupSize(work_groups);
                    fprintf(stderr,
                            "Starting simulation from file %s with %d simulation steps and work_group_size %d. Running with OpenCL. Writing to file %s\n",
                            input_file_path.c_str(), simulation_steps, work_groups, output_file_path.c_str());
                }
                simulator->startSimulation(simulation_steps);
            } else {
                if (simulator_selector.compare("ocl") == 0) {
                    simulator = new OpenClSimulator(input_file_path);
                    fprintf(stderr,
                            "Starting simulation from file %s with %d simulation steps. Running with OpenCl. Writing to console...\n",
                            input_file_path.c_str(), simulation_steps);
                } else if (simulator_selector.compare("omp") == 0) {
                    simulator = new OmpSimulator(input_file_path);
                    fprintf(stderr,
                            "Starting simulation from file %s with %d simulation steps. Running with OpenMP. Writing to console...\n",
                            input_file_path.c_str(), simulation_steps);
                } else {
                    throw std::runtime_error(std::string("Unknown simulator ") + simulator_selector);
                }
                simulator->startSimulation(simulation_steps);
            }
        } else {
            throw std::runtime_error("Invalid amount of arguments\n");
        }
    } catch (std::exception &e) {
        fprintf(stderr, e.what());
        fprintf(stderr,
                "Provide IN_FILE, SIMULATION_STEPS, SIMULATOR_SELECTOR (and optionally OUT_FILE) as arguments!\n"
                        "\t- select -1 simulation steps to run indefinitely\n"
                        "\t- possible simulator selectors are: omp, ocl");
        result = EXIT_FAILURE;
    }

    delete (simulator);
    return result;
}
