#!/bin/bash

# Clear everything
make remove;

# Rebuild
make;

# We are cd'ing into the bin folder because otherwise the executable
# cannot find the kernels in the "kernel" folder
cd bin;
./nbody_opencl;
cd ..;