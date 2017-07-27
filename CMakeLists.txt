cmake_minimum_required(VERSION 3.7)

project(nbody_psys17)

# Warning: It is required to set CXX environment variable to g++-6 compiler
if(NOT (CMAKE_CXX_COMPILER MATCHES "g\\++-6"))
  message(FATAL_ERROR "Make sure that CXX environment variable is set to g++-6 compiler! Currently CMake uses '${CMAKE_CXX_COMPILER}'. Set CXX environment variable, clear CMake caches and try again.")
endif()

set(CMAKE_CXX_FLAGS "-framework OpenCL -std=c++11 -fopenmp -ggdb")
set(CMAKE_EXE_LINKER_FLAGS "-Wall -I. -lm")
set(CMAKE_CXX_STANDARD 11)

find_package( OpenCL REQUIRED )

include_directories(includes ${OPENCL_INCLUDE_DIR})

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
        src/main/Mode.h)

# add_subdirectory(src/test)

add_executable(nbody_psys17 ${SOURCE_FILES})
target_link_libraries(nbody_psys17 ${OPENCL_LIBRARIES})
