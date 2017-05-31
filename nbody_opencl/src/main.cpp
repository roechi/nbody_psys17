#include <stdio.h>
#include "../includes/cl.hpp"
#include <iostream>
#include <fstream>
#include "util.hpp"

int main() {
    srand((unsigned int)time(NULL));

//    const float UPDATE_STEP = 1.0f / 365.25f;
    const float UPDATE_STEP = 1.0f / 4000.25f;
    const float RADIUS_UNIVERSE = 3.0;
    size_t num_bodies = 1000;

    float masses[num_bodies] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float positions[num_bodies*2] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float velocities[num_bodies*2] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float forces[num_bodies*2] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    util::init_bodies(positions,masses,velocities,forces,num_bodies);
//    for (int j = 0; j < num_bodies; ++j) {
//        printf("%e,%e\t",positions[2*j+0],positions[2*j+1]);
//    }
//    printf("\n");

//
//    for (int j = 0; j < num_bodies; ++j) {
//        printf("Mass: %e\n",masses[j]);
//        printf("Position: (%e,%e)\n",positions[2*j]*250/1e18,positions[2*j+1]*250/1e18);
//        printf("Velocities: (%e,%e)\n",velocities[2*j],velocities[2*j+1]);
//        printf("Forces: (%e,%e)\n",forces[2*j],forces[2*j+1]);
//        printf("\n");
//    }

    
    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cerr <<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    std::cerr  << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
    if(all_devices.size()==0){
        std::cerr <<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cerr << "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";


    cl::Context context(default_device);

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code=util::read_file_contents("../kernel/core.cl");

    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cerr <<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }


    // create buffers on the device
    cl::Buffer buffer_masses(context,CL_MEM_READ_WRITE,sizeof(float)*num_bodies);
    cl::Buffer buffer_positions(context,CL_MEM_READ_WRITE,sizeof(float)*num_bodies*2);
    cl::Buffer buffer_velocities(context,CL_MEM_READ_WRITE,sizeof(float)*num_bodies*2);

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_masses,CL_TRUE,0,sizeof(float)*num_bodies,masses);
    queue.enqueueWriteBuffer(buffer_positions,CL_TRUE,0,sizeof(float)*num_bodies*2,positions);
    queue.enqueueWriteBuffer(buffer_velocities,CL_TRUE,0,sizeof(float)*num_bodies*2,velocities);


    //run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"nbody_move");
    kernel_add.setArg(0,buffer_masses);
    kernel_add.setArg(1,buffer_positions);
    kernel_add.setArg(2,buffer_velocities);
    kernel_add.setArg(3,num_bodies);
    kernel_add.setArg(4,UPDATE_STEP);

    for (int i = 0; i < 10000; ++i) {

        queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(num_bodies),cl::NullRange);
        queue.finish();

        queue.enqueueReadBuffer(buffer_masses,CL_TRUE,0,sizeof(float)*num_bodies,masses);
        queue.enqueueReadBuffer(buffer_positions,CL_TRUE,0,sizeof(float)*num_bodies*2,positions);
        queue.enqueueReadBuffer(buffer_velocities,CL_TRUE,0,sizeof(float)*num_bodies*2,velocities);

        for (int j = 0; j < num_bodies; ++j) {
            fprintf(stdout,"%e\t%e\t%e\t",positions[2*j+0]/(RADIUS_UNIVERSE*10.0f),positions[2*j+1]/(RADIUS_UNIVERSE*10.0f),masses[j]);
        }
        printf("\n");
    }

    return 0;
}