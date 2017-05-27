/*
 * Simulator.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "Simulator.h"
#include <random>
#include "Body.h"
#include <chrono>
#include <iostream>
#include <fstream>

Simulator::Simulator(std::string file_path) {
    this->bodies = new Body[Simulator::NUM_BODIES];
    file.open(file_path);
}

void Simulator::startSimulation() {
    this->generateBodies();
    this->loop();
    this->file.close();
}

void Simulator::startSimulation(int num_bodies, int simulation_steps)
{
    this->NUM_BODIES= num_bodies;
    this->SIMULATION_STEPS = simulation_steps;
    startSimulation();
}

void Simulator::loop() {

    for (int step = 0; step < this->SIMULATION_STEPS; step++){
        for(int i=0; i<this->NUM_BODIES; i++) {
            double x = this->bodies[i].rx / RADIUS_UNIVERSE;
            double y = this->bodies[i].ry / RADIUS_UNIVERSE;
            double mass = this->bodies[i].m;
            file << x << "\t" << y << "\t" << mass << "\t";
        }
        file << "\n";
        addForces();
    }

}

void Simulator::generateBodies() {

    // sun
    bodies[0] = Body(Simulator::SOLAR_MASS, 0.0000e+00,  0.0000e+00,  0.0000e+00,  0.0000e+00);

    // mercury
    bodies[1] = Body(3.302e+23 / Simulator::MASS_SCALE, 57909175e3 / Simulator::LENGTH_SCALE,  0.0000e+00,  0.0000e+00,  (47.8725e+3  / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE);

    // venus
    bodies[2] = Body(4.8690e+24 / Simulator::MASS_SCALE, 108208930e3 / Simulator::LENGTH_SCALE,  0.0000e+00,  0.0000e+00,  (35.0214e+3  / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE);

    // earth
    bodies[3] = Body(5.9742e+24 / Simulator::MASS_SCALE, 149597890e3 / Simulator::LENGTH_SCALE,  0.0000e+00,  0.0000e+00,   (29.7859e+3  / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE);

    // mars
    bodies[4] = Body(6.4191e+23 / Simulator::MASS_SCALE, 227936640e3 / Simulator::LENGTH_SCALE,  0.0000e+00,  0.0000e+00,  (24.1309e+3  / Simulator::LENGTH_SCALE) * Simulator::TIME_SCALE);


    for (int i = 0; i < Simulator::NUM_BODIES; ++i) {
        bodies[i].print();
    }

//    // jupiter
//    bodies[5] = Body(1.8987e+27, 778412010e3,  0.0000e+00,  0.0000e+00,  1.30697e+4);
//
//    // saturn
//    bodies[6] = Body(5.6851e+26, 1426725400e3,  0.0000e+00,  0.0000e+00,  0.96724e+4);
//
//    // uranus
//    bodies[7] = Body(8.6849e+25, 2870972200e3,  0.0000e+00,  0.0000e+00,  0.68352e+4);
//
//    // neptune
//    bodies[8] = Body(1.0244e+26, 4498252900e3,  0.0000e+00,  0.0000e+00,  0.54778e+4);
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

