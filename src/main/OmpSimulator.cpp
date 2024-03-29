/*
 * Simulator.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "OmpSimulator.h"
#include "SignalHandler.h"
#include <random>
#include <chrono>

OmpSimulator::OmpSimulator(const std::string &input_file_path, const std::string &output_file_path)
        : Simulator(input_file_path, output_file_path) {
    initializeBodies();
}

OmpSimulator::OmpSimulator(const std::string &input_file_path)
        : Simulator(input_file_path) {
    initializeBodies();
}

void OmpSimulator::initializeBodies() {
    this->generateBodies();
    this->scaleBodies();
}

int OmpSimulator::startSimulation(int simulation_steps) {
    int exit_status = this->loop(simulation_steps);
    if (this->mode == Mode::FILE) {
        this->output_file.close();
    }

    delete[](this->bodies);

    return exit_status;
}

void OmpSimulator::generateBodies() {
    ConfigParser *parser = new ConfigParser();
    int numberOfLines = parser->getNumberOfLines(this->input_file_path);
    this->num_bodies = numberOfLines;
    this->bodies = new Body[this->num_bodies];
    std::list<Body> parsedBodies = parser->parseFile(this->input_file_path);
    delete parser;

    int k = 0;
    for (Body b : parsedBodies) {
        bodies[k] = b;
        k++;
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

int OmpSimulator::loop(int simulation_steps) {
    int iret = EXIT_SUCCESS;

    if (simulation_steps != -1) {
        for (int step = 0; step < simulation_steps; step++) {
            runStep();
        }
    } else {
        try {
            SignalHandler signalHandler;
            signalHandler.setupSignalHandlers();

            while (!signalHandler.gotExitSignal()) {
                runStep();
            }
        } catch (SignalException &e) {
            std::cerr << e.what();
            iret = EXIT_FAILURE;
        }
    }

    return iret;
}

void OmpSimulator::runStep() {
    for (int i = 0; i < num_bodies; i++) {
        double x = bodies[i].rx / RADIUS_UNIVERSE;
        double y = bodies[i].ry / RADIUS_UNIVERSE;
        double mass = bodies[i].m;
        if (this->mode == Mode::FILE) {
            output_file << x << "\t" << y << "\t" << mass << "\t";
        } else if (this->mode == Mode::CONSOLE) {
            std::cout << x << "\t" << y << "\t" << mass << "\t";
        }
    }
    if (this->mode == Mode::FILE) {
        output_file << "\n";
    } else {
        std::cout << "\n";
    }
    addForces();
}

void OmpSimulator::addForces() {

#pragma omp parallel for
    for (int k = 0; k < this->num_bodies; ++k) {
        this->bodies[k].resetForce();
    }

#pragma omp parallel for collapse(2)
    for (int i = 0; i < this->num_bodies; i++) {
        for (int j = 0; j < this->num_bodies; j++) {
            if (i != j) {
                this->bodies[i].addForce(this->bodies[j], Simulator::GRAVITATIONAL_CONSTANT, 0.1f);
            }
        }
    }

#pragma omp parallel for
    for (int i = 0; i < this->num_bodies; i++) {
        this->bodies[i].update(UPDATE_STEP);
    }
}



