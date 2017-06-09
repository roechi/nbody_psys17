cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_FLAGS "-framework OpenCL -std=c99 -Wall -I. -fopenmp -ggdb")
set(CMAKE_CXX_FLAGS "-framework OpenCL -std=c++11 -fopenmp -ggdb")
set(CMAKE_EXE_LINKER_FLAGS " -Wall -I. -lm -fopenmp")

project(nbody_psys17)

add_subdirectory(src/test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main/Body.cpp
        src/main/Body.h
        src/main/main.cpp
        src/main/OmpSimulator.cpp
        src/main/OmpSimulator.h
        src/main/ConfigParser.cpp
        src/main/ConfigParser.h
        src/main/Simulator.h
        src/main/OpenClSimulator.cpp
        src/main/OpenClSimulator.h
        src/main/Util.cpp
        src/main/Util.h
        src/main/SignalHandler.cpp
        src/main/SignalHandler.h
        )

include_directories(/usr/local/Cellar/gcc/6.3.0_1/lib/gcc/6/gcc/x86_64-apple-darwin16.4.0/6.3.0/include includes)
add_executable(nbody_psys17 ${SOURCE_FILES})