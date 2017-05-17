cmake_minimum_required(VERSION 3.7)

project(nbody_opencl)
set(CMAKE_C_FLAGS "-framework OpenCL -std=c99 -Wall -I. -ggdb")
set(CMAKE_CXX_FLAGS "-framework OpenCL -std=c++11 -ggdb")
set(CMAKE_EXE_LINKER_FLAGS " -Wall -I. -lm")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/util.cpp src/util.hpp)

include_directories(/usr/local/Cellar/gcc/6.3.0_1/lib/gcc/6/gcc/x86_64-apple-darwin16.4.0/6.3.0/include)
include_directories(includes)

add_executable(nbody_opencl ${SOURCE_FILES})