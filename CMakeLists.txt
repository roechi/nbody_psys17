cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_FLAGS "-std=c99 -Wall -I. -fopenmp -ggdb")
set(CMAKE_CXX_FLAGS "-std=c++11 -fopenmp -ggdb")
set(CMAKE_EXE_LINKER_FLAGS " -Wall -I. -lm -fopenmp")

project(nbody_psys17)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Body.cpp
        src/Body.h
        src/main.cpp
        src/Simulator.cpp
        src/Simulator.h)

include_directories(/usr/local/Cellar/gcc/6.3.0_1/lib/gcc/6/gcc/x86_64-apple-darwin16.4.0/6.3.0/include)

add_executable(nbody_psys17 ${SOURCE_FILES})