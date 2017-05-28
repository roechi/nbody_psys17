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
    std::string delimiter = " ";

    auto start = 0U;
    auto end = line.find(delimiter);
    double* arr = new double[5];
    int pos = 0;
    while (end != std::string::npos)
    {
        std::string parameter = line.substr(start, end - start);

        arr[pos] = std::stod(parameter);
        pos++;

        start = end + delimiter.length();
        end = line.find(delimiter, start);
    }

    arr[4] = stod(line.substr(start, end - start));

    return Body(arr[0], arr[1], arr[2], arr[3], arr[4]);
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