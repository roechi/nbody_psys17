cmake_minimum_required(VERSION 3.7)

project(nbody_opencl)
set(CMAKE_C_FLAGS "-framework OpenCL -std=c99 -Wall -I. -ggdb")
set(CMAKE_CXX_FLAGS "-std=c++11 -ggdb")
set(CMAKE_EXE_LINKER_FLAGS " -Wall -I. -lm")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.c)

add_executable(nbody_opencl ${SOURCE_FILES})