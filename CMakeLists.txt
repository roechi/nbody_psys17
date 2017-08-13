cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_COMPILER /usr/bin/g++-6)

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "-IOpenCL -std=c++11 -fopenmp -ggdb")
else()
    set(CMAKE_CXX_FLAGS "-framework OpenCL -std=c++11 -fopenmp -ggdb")
endif()

set(CMAKE_EXE_LINKER_FLAGS " -Wall -I. -lm")
set(CMAKE_CXX_STANDARD 11)

project(nbody_psys17)

add_subdirectory(src/test)

find_package(OpenCL REQUIRED)

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
        src/main/Mode.cpp
        src/main/Mode.h
        )

include_directories(includes)
add_executable(nbody_psys17 ${SOURCE_FILES})
target_link_libraries(nbody_psys17 ${OpenCL_LIBRARIES})
