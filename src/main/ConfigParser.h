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
    std::list<Body> parseFile(std::string file_path);
    Body parseLine(std::string line);

private:
    std::list<std::string> read_file(std::istream& istream);
};


#endif //NBODY_PSYS17_CONFIGPARSER_H
