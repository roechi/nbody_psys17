//
// Created by Richard Remus on 10.05.17.
//

#ifndef NBODY_PSYS17_CONFIGPARSER_H
#define NBODY_PSYS17_CONFIGPARSER_H


#include "Body.h"
#include <string>
#include <list>

class ConfigParser {

public:

    /**
     * Parses the given config file at path file_path into a list of Body
     * @param file_path The path to the config file
     * @return A list of Body, according to the contents in the config file
     */
    std::list<Body> parseFile(std::string file_path);


    /**
     * Parses the given config file at path file_path and writes the initial values in the float[] arrays accordingly
     * @param file_path The path to the config file
     * @return void, the actual values are saved in the float[] arrays given as parameters
     */
    void parseFile(std::string file_path, float* masses, float* positions, float* velocities, float* forces);


    /**
     * Gets the number of lines in the file at path file_path, i.e. the number of bodies in the file
     * @param file_path The path to the config file
     * @return the number of lines in the file, i.e. the number of bodies
     */
    int getNumberOfLines(std::string file_path);

private:
    Body parseLine(std::string line);
    std::list<std::string> read_file(std::istream& istream);
};


#endif //NBODY_PSYS17_CONFIGPARSER_H
