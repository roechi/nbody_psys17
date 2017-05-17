#include <stdio.h>
#include "../includes/cl.hpp"
#include <iostream>
#include <fstream>
#include "util.hpp"

int main() {
    const size_t vec_length = 10;

    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";


    cl::Context context(default_device);

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code=util::read_file_contents("../kernel/core.cl");

    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }


    // create buffers on the device
    cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(float)*vec_length);
    cl::Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(float)*vec_length);
    cl::Buffer buffer_C(context,CL_MEM_READ_WRITE,sizeof(float)*vec_length);

    float A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(float)*vec_length,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(float)*vec_length,B);


    //run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
    kernel_add.setArg(0,buffer_A);
    kernel_add.setArg(1,buffer_B);
    kernel_add.setArg(2,buffer_C);
    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(vec_length),cl::NullRange);
    queue.finish();

    float C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(float)*vec_length,C);

    for(int i=0;i<vec_length;i++){
        std::cout<<C[i]<<" ";
    }

    return 0;
}