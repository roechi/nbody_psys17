/*
 * Simulator.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "OmpSimulator.h"
#include "Simulator.h"
#include <random>
#include "Body.h"
#include <chrono>
#include <iostream>
#include <fstream>


OmpSimulator::OmpSimulator(const std::string &input_file_path, const std::string &output_file_path, int simulation_steps)
        : Simulator(input_file_path, output_file_path, simulation_steps) {

}

void OmpSimulator::initializeBodies() {
    this->bodies = parser.parseFile(this->input_file_path);
}

void OmpSimulator::startSimulation() {
    this->generateBodies();
    this->scaleBodies();
    this->loop();
}

void OmpSimulator::generateBodies() {
    ConfigParser *parser = new ConfigParser();
    int numberOfLines = parser->getNumberOfLines(this->input_file_path);
    std::list<Body> parsedBodies = parser->parseFile(this->input_file_path);

    /**
     * Set the number of bodies according to the number of lines in the config file
     */
    this->num_bodies = numberOfLines;

    int k = 0;
    for (Body b : parsedBodies) {
        bodies[k] = b;
        k++;
    }

    scaleBodies();

    for (int i = 0; i < this->num_bodies; ++i) {
        bodies[i].print();
    }
}

void OmpSimulator::scaleBodies() {
    for (int i = 0; i < this->num_bodies; ++i) {
        bodies[i].m /= Simulator::MASS_SCALE;
        bodies[i].rx /= Simulator::LENGTH_SCALE;
        bodies[i].ry /= Simulator::LENGTH_SCALE;
        bodies[i].vx = (bodies[i].vx / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE;
        bodies[i].vy = (bodies[i].vy / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE;
    }
}

void OmpSimulator::loop() {

    for (int step = 0; step < this->SIMULATION_STEPS; step++){
        for(int i=0; i < this->num_bodies; i++) {
            double x = this->bodies[i].rx / RADIUS_UNIVERSE;
            double y = this->bodies[i].ry / RADIUS_UNIVERSE;
            double mass = this->bodies[i].m;
            file << x << "\t" << y << "\t" << mass << "\t";
        }
        file << "\n";
        addForces();
    }

}

void OmpSimulator::addForces() {

    #pragma omp parallel for
    for (int k = 0; k < this->num_bodies; ++k) {
        this->bodies[k].resetForce();
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i <  this->num_bodies; i++) {
      for (int j = 0; j <  this->num_bodies; j++) {
        if (i != j) {
            this->bodies[i].addForce(this->bodies[j]);
        }
      }
    }

    #pragma omp parallel for
    for (int i = 0; i < this->num_bodies; i++) {
      this->bodies[i].update(UPDATE_STEP);
    }
}



