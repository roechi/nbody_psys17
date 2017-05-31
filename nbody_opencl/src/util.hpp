//
// Created by Jonas Jaszkowic on 17.05.17.
//

#include <string>
#include <fstream>

namespace util {
    std::string read_file_contents(std::string filename);
    void init_bodies(float* positions, float* masses, float* velocities, float* forces,size_t num_bodies);
    float getRandomInInterval(float lower, float upper);
    float circularVelocity(float rx, float ry);
    float signum(float val);
//    float exp(float lambda);
};