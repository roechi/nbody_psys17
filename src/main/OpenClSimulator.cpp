//
// Created by Jonas Jaszkowic on 08.06.17.
//

#include <assert.h>
#include "OpenClSimulator.h"
#include "../includes/cl.hpp"
#include "Util.h"
#include "SignalHandler.h"

OpenClSimulator::OpenClSimulator(const std::string &input_file_path, const std::string &output_file_path)
        : Simulator(input_file_path, output_file_path) {
//    output_file.open(output_file_path);
    initializeBodies();
    scaleBodies();
}

OpenClSimulator::OpenClSimulator(const std::string &input_file_path)
        : Simulator(input_file_path) {
//    output_file.open(output_file_path);
    initializeBodies();
    scaleBodies();
}

void OpenClSimulator::initializeBodies() {
    ConfigParser *parser = new ConfigParser();
    int numberOfLines = parser->getNumberOfLines(this->input_file_path);
    this->num_bodies = numberOfLines;
    this->work_group_size = 1; // Default: Maximum parallelization

    masses = new float[this->num_bodies];
    positions = new float[this->num_bodies * 2];
    velocities = new float[this->num_bodies * 2];
    forces = new float[this->num_bodies * 2];

    // parse the values in the float arrays
    parser->parseFile(this->input_file_path, masses, positions, velocities, forces);
}

void OpenClSimulator::scaleBodies() {
    for (int i = 0; i < this->num_bodies; ++i) {
        int idx_1d = i;
        int idx_2d = 2 * i;
        masses[idx_1d] /= Simulator::MASS_SCALE;
        positions[idx_2d] /= Simulator::LENGTH_SCALE;
        positions[idx_2d + 1] /= Simulator::LENGTH_SCALE;
        velocities[idx_2d] = (velocities[idx_2d] / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE;
        velocities[idx_2d + 1] = (velocities[idx_2d + 1] / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE;
    }
}

int OpenClSimulator::startSimulation(int simulation_steps) {
//    assert(this->num_bodies % work_group_size == 0);

    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0) {
        std::cerr << " No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform = all_platforms[0];
//    std::cerr << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
    if (all_devices.size() == 0) {
        std::cerr << " No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device = all_devices[0];
//    std::cerr << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";


    cl::Context context(default_device);

    cl::Program::Sources sources;
    std::string kernel_code = Util::read_file_contents("../kernel/core.cl");
    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program program(context, sources);
    if (program.build({default_device}) != CL_SUCCESS) {
        std::cerr << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
        exit(1);
    }


    // create buffers on the device
    cl::Buffer buffer_masses(context, CL_MEM_READ_WRITE, sizeof(float) * num_bodies);
    cl::Buffer buffer_positions(context, CL_MEM_READ_WRITE, sizeof(float) * num_bodies * 2);
    cl::Buffer buffer_velocities(context, CL_MEM_READ_WRITE, sizeof(float) * num_bodies * 2);
    cl::Buffer buffer_forces(context, CL_MEM_READ_WRITE, sizeof(float) * num_bodies * 2 );

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context, default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_masses, CL_TRUE, 0, sizeof(float) * num_bodies, masses);
    queue.enqueueWriteBuffer(buffer_positions, CL_TRUE, 0, sizeof(float) * num_bodies * 2, positions);
    queue.enqueueWriteBuffer(buffer_velocities, CL_TRUE, 0, sizeof(float) * num_bodies * 2, velocities);
    queue.enqueueWriteBuffer(buffer_forces,CL_TRUE,0,sizeof(float) * num_bodies * 2, forces);

    //run the kernel
    const float update = Simulator::UPDATE_STEP;
    cl::Kernel kernel_add = cl::Kernel(program, "nbody_move");
    kernel_add.setArg(0, buffer_masses);
    kernel_add.setArg(1, buffer_positions);
    kernel_add.setArg(2, buffer_velocities);
    kernel_add.setArg(3, buffer_forces);
    kernel_add.setArg(4, num_bodies);
    kernel_add.setArg(5, update);
    kernel_add.setArg(6, work_group_size);
//    fprintf(stderr, "work_group_size=%d", work_group_size);

    int iret = EXIT_SUCCESS;

    if (simulation_steps != -1) {
        for (int i = 0; i < simulation_steps; ++i) {
            runStep(buffer_masses, buffer_positions, buffer_velocities, queue, kernel_add);
        }
    } else {
        try {
            SignalHandler signalHandler;
            signalHandler.setupSignalHandlers();

            while (!signalHandler.gotExitSignal()) {
                runStep(buffer_masses, buffer_positions, buffer_velocities, queue, kernel_add);
            }

        } catch (SignalException &e) {
            std::cerr << e.what();
            iret = EXIT_FAILURE;
        }
    }

    if (this->mode == Mode::FILE) {
        this->output_file.close();
    }

    return iret;
}

void OpenClSimulator::runStep(cl::Buffer &buffer_masses, cl::Buffer &buffer_positions,
                              cl::Buffer &buffer_velocities, cl::CommandQueue &queue,
                              cl::Kernel &kernel_add) {
  //  uint global_size = (uint)this->num_bodies / this->work_group_size;
//    fprintf(stderr, "global_size=%i\n",global_size);
//    fprintf(stderr, "local_size=%i\n",work_group_size);
    queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(this->num_bodies), cl::NullRange);
    queue.finish();

    queue.enqueueReadBuffer(buffer_masses, CL_TRUE, 0, sizeof(float) * this->num_bodies, masses);
    queue.enqueueReadBuffer(buffer_positions, CL_TRUE, 0, sizeof(float) * this->num_bodies * 2, positions);
    queue.enqueueReadBuffer(buffer_velocities, CL_TRUE, 0, sizeof(float) * this->num_bodies * 2, velocities);

    for (int j = 0; j < this->num_bodies; ++j) {
        if (this->mode == Mode::FILE) {
            this->output_file << positions[2 * j + 0] / RADIUS_UNIVERSE << "\t" << positions[2 * j + 1] /
                                                                                   RADIUS_UNIVERSE << "\t" << masses[j]
                              << "\t";
        } else if (this->mode == Mode::CONSOLE) {
            std::cout << positions[2 * j + 0] / RADIUS_UNIVERSE << "\t" << positions[2 * j + 1] /
                                                                                   RADIUS_UNIVERSE << "\t" << masses[j]
                              << "\t";
        }
    }

    if (this->mode == Mode::FILE) {
        this->output_file << "\n";
    } else if (this->mode == Mode::CONSOLE){
        std::cout << "\n";
    }
}

void OpenClSimulator::setWorkGroupSize(uint work_group_size) {
    this->work_group_size = work_group_size;
}
