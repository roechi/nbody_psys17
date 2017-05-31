//
// Created by Jonas Jaszkowic on 17.05.17.
//

#include <math.h>
#include "util.hpp"
#include <assert.h>

namespace util {
    /**
     * Read the content of the file given by the filename.
     *
     * @param filename the name of the file which should be read.
     *
     * @return the contents or NULL on an error. The returned
     *  value must be freed with free() when no longer in use.
     */
    std::string read_file_contents(std::string filename)
    {
        std::ifstream ifs(filename);
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        return content;
    }

    void init_bodies(float* positions, float* masses, float* velocities, float* forces, size_t num_bodies) {

        // Time is measured in years, distances in AU and masses in solar-masses
        static const float RADIUS_UNIVERSE = 3.0;
        static const float SOLAR_MASS = 1.0;
//        static const float GRAVITATIONAL_CONSTANT = 39.5;
//        static const float UPDATE_STEP = 1.0f / 365.25f; // Update step 1 day
        static const float MASS_SCALE = 1.98892e+30; // 1 Solar Mass in kg
        static const float LENGTH_SCALE =  149597870700; // 1 AU in m
        static const float TIME_SCALE = 86400.0f * 365.25f; // 1 year in seconds
//        static const float PI = 3.14159265359f;


//        // sun
//        masses[0] = SOLAR_MASS;
//        positions[0] = 0.0f; positions[1] = 0.0f;
//        velocities[0] = 0.0f; velocities[1] = 0.0f;
//
//        // mercury
//        masses[1] = 3.302e+23f / MASS_SCALE;
//        positions[2] = 57909175e3f / LENGTH_SCALE; positions[3] = 0.0f;
//        velocities[2] = 0.0f; velocities[3] = (47.8725e+3f  / LENGTH_SCALE) * TIME_SCALE;
//
//        // venus
//        masses[2] = 4.8690e+24f / MASS_SCALE;
//        positions[4] =  108208930e3f / LENGTH_SCALE; positions[5] = 0.0f;
//        velocities[4] = 0.0f; velocities[5] =  (35.0214e+3f  / LENGTH_SCALE) * TIME_SCALE;
//
//        // earth
//        masses[3] = 5.9742e+24f / MASS_SCALE;
//        positions[6] =  149597890e3f / LENGTH_SCALE; positions[7] = 0.0f;
//        velocities[6] = 0.0f; velocities[7] = (29.7859e+3f  / LENGTH_SCALE) * TIME_SCALE;
//
//        // mars
//        masses[4] = 6.4191e+23f / MASS_SCALE;
//        positions[8] =  227936640e3f / LENGTH_SCALE; positions[9] = 0.0f;
//        velocities[8] = 0.0f; velocities[9] = (24.1309e+3f  / LENGTH_SCALE) * TIME_SCALE;

        for (int i = 0; i < num_bodies; ++i) {
            masses[i] = 1.0f;
            positions[i*2] = getRandomInInterval(-RADIUS_UNIVERSE, RADIUS_UNIVERSE);
            positions[i*2+1] = getRandomInInterval(-RADIUS_UNIVERSE, RADIUS_UNIVERSE);
            velocities[i*2] = 0.0f;
            velocities[i*2+1] = 0.0f;
        }

        masses[0] = 100.0f;
        positions[0] = 0.0f;
        positions[1] = 0.0f;
        velocities[0] = 0.0f;
        velocities[1] = 0.0f;
    }

    float getRandomInInterval(float lower, float upper) {
        float res =  lower +(float)rand()/(float)(RAND_MAX/(upper-lower));
        assert(res >= lower);
        assert(res <= upper);
        return res;
    }

}
