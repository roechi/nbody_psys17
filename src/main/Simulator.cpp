/*
 * Simulator.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "Simulator.h"
#include <random>
#include <chrono>

Simulator::Simulator(std::string input_file_path, std::string output_file_path) {
    file.open(output_file_path);
    this->input_file_path = input_file_path;
    this->generateBodies();
}

void Simulator::startSimulation(int simulation_steps)
{
    this->SIMULATION_STEPS = simulation_steps;
    startSimulation();
}

void Simulator::startSimulation() {
    this->loop();
    this->file.close();
}

void Simulator::generateBodies() {
    ConfigParser *parser = new ConfigParser();
    int numberOfLines = parser->getNumberOfLines(this->input_file_path);
    this->NUM_BODIES = numberOfLines;
    this->bodies = new Body[Simulator::NUM_BODIES];
    std::list<Body> parsedBodies = parser->parseFile(this->input_file_path);


    int k = 0;
    for (Body b : parsedBodies) {
        bodies[k] = b;
        k++;
    }

    scaleBodies();

    for (int i = 0; i < Simulator::NUM_BODIES; ++i) {
        bodies[i].print();
    }
}

void Simulator::scaleBodies() {
    for (int i = 0; i < Simulator::NUM_BODIES; ++i) {
        bodies[i].m /= Simulator::MASS_SCALE;
        bodies[i].rx /= Simulator::LENGTH_SCALE;
        bodies[i].ry /= Simulator::LENGTH_SCALE;
        bodies[i].vx = (bodies[i].vx / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE;
        bodies[i].vy = (bodies[i].vy / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE;
    }
}

void Simulator::loop() {

    for (int step = 0; step < this->SIMULATION_STEPS; step++){
        for(int i=0; i < this->NUM_BODIES; i++) {
            double x = this->bodies[i].rx / RADIUS_UNIVERSE;
            double y = this->bodies[i].ry / RADIUS_UNIVERSE;
            double mass = this->bodies[i].m;
            file << x << "\t" << y << "\t" << mass << "\t";
        }
        file << "\n";
        addForces();
    }

}

double Simulator::exp(double lambda) {
        return -log(1 - getRandom()) / lambda;
}

void Simulator::addForces() {

    #pragma omp parallel for
    for (int k = 0; k < Simulator::NUM_BODIES; ++k) {
        this->bodies[k].resetForce();
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i <  this->NUM_BODIES; i++) {
      for (int j = 0; j <  this->NUM_BODIES; j++) {
        if (i != j) {
            this->bodies[i].addForce(this->bodies[j]);
        }
      }
    }

    #pragma omp parallel for
    for (int i = 0; i < Simulator::NUM_BODIES; i++) {
      this->bodies[i].update(UPDATE_STEP);
    }
}

double Simulator::getRandom() {
    double r = ((double) rand() / (RAND_MAX));
    return r;
}

double Simulator::circularVelocity(double rx, double ry) {
    double r2=sqrt(rx*rx+ry*ry);
    double numerator=(GRAVITATIONAL_CONSTANT)*1e6*SOLAR_MASS;
    return sqrt(numerator/r2);
}

double Simulator::signum(double val) {
    return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
}

