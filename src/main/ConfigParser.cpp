//
// Created by Richard Remus on 10.05.17.
//

#include <iostream>
#include <fstream>
#include "ConfigParser.h"

std::list<std::string> ConfigParser::read_file(std::istream& istream)
{
    std::list<std::string> list = std::list<std::string>();
    while (!istream.eof())
    {
        std::string line;
        std::getline(istream, line);
        if(!istream) break;
        list.push_front(line);
    }
    return list;
}

std::list<Body> ConfigParser::parseFile(std::string file_path)
{
    std::ifstream ifs(file_path);
    if (!ifs.good())
    {
        throw (std::string("Could not read file at path ") + file_path);
    }

    std::list<std::string> lines = read_file(ifs);

    std::list<Body> bodies = std::list<Body>();

    for (std::list<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        bodies.push_back(parseLine(*it));
    }

    return bodies;
}

Body ConfigParser::parseLine(std::string line)
{
    float* values = parseLineRaw(line);
    return Body(values[0], values[1], values[2], values[3], values[4]);
}


void ConfigParser::parseFile(std::string file_path, float* masses, float* positions, float* velocities, float* forces) {
    std::ifstream ifs(file_path);
    if (!ifs.good())
    {
        throw (std::string("Could not read file at path ") + file_path);
    }

    int num_lines = getNumberOfLines(file_path);

    std::list<std::string> lines = read_file(ifs);

    int idx = num_lines-1;
    for (std::list<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        std::string line = *it;
        float* values = parseLineRaw(*it);

        // Set the values directly in the array
        masses[idx] = values[0];

        positions[idx*2] = values[1];
        positions[idx*2+1] = values[2];

        velocities[idx*2] = values[3];
        velocities[idx*2+1] = values[4];

        forces[idx*2] = 0.0f;
        forces[idx*2+1] = 0.0f;

        idx--;
    }
}

float* ConfigParser::parseLineRaw(std::string line) {
    std::string delimiter = " ";

    auto start = 0U;
    auto end = line.find(delimiter);
    float* arr = new float[5];
    int pos = 0;
    while (end != std::string::npos)
    {
        std::string parameter = line.substr(start, end - start);

        arr[pos] = std::stof(parameter);
        pos++;

        start = end + delimiter.length();
        end = line.find(delimiter, start);
    }

    arr[4] = stof(line.substr(start, end - start));

    return arr;
}


int ConfigParser::getNumberOfLines(std::string file_path) {
    std::ifstream ifs(file_path);
    if (!ifs.good())
    {
        throw (std::string("Could not read file at path ") + file_path);
    }

    std::list<std::string> lines = read_file(ifs);
    return (int) lines.size();
}