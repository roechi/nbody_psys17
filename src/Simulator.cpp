/*
 * Simulator.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "Simulator.h"
#include <random>
#include "Body.h"

Simulator::Simulator() {
	this->bodies = new Body[Simulator::NUM_BODIES];
}

void Simulator::startSimulation() {
	this->generateBodies();
	this->loop();
}

void Simulator::loop() {

	for (int step = 0; step < Simulator::SIMULATION_STEPS; step++){
	    for(int i=0; i<Simulator::NUM_BODIES; i++) {
	    	double x = this->bodies[i].rx * 250 / RADIUS_UNIVERSE;
	    	double y = this->bodies[i].ry * 250 / RADIUS_UNIVERSE;
	    	double mass = this->bodies[i].m;
	    	printf("%f\t%f\t%f\t", x, y, mass);
	    }
	    printf("\n");
	    addForces();
	}

}

void Simulator::generateBodies() {
    //Put the central mass in
    this->bodies[0] = Body(1e6*SOLAR_MASS, 0, 0, 0,0);//put a heavy body in the center

	for (int i = 1; i < NUM_BODIES; i++) {
	  double px = RADIUS_UNIVERSE*exp(-1.8)*(0.5-getRandom());
	  double py = RADIUS_UNIVERSE*exp(-1.8)*(0.5-getRandom());

	  double magv = circlev(px, py);

	  double absangle = atan(abs(py/px));
	  double thetav= M_PI/2-absangle;
	  double phiv = getRandom()*M_PI;
	  double vx   = -1*signum(py)*cos(thetav)*magv;
	  double vy   = signum(px)*sin(thetav)*magv;

		if (getRandom() <= 0.5) {
		  vx=-vx;
		  vy=-vy;
		}

	  double mass = getRandom()*SOLAR_MASS*10+1e20;

	  this->bodies[i] = Body(mass, px,py, vx, vy);
	}
}

double Simulator::exp(double lambda) {
        return -log(1 - getRandom()) / lambda;
}

void Simulator::addForces() {
    for (int i = 0; i < Simulator::NUM_BODIES; i++) {
      this->bodies[i].resetForce();
      for (int j = 0; j < Simulator::NUM_BODIES; j++) {
        if (i != j) {
        	this->bodies[i].addForce(this->bodies[j]);
        }
      }
    }
    for (int i = 0; i < Simulator::NUM_BODIES; i++) {
      this->bodies[i].update(UPDATE_STEP);
    }
}

double Simulator::getRandom() {
	double r = ((double) rand() / (RAND_MAX));
	return r;
}

double Simulator::circlev(double rx, double ry) {
    double r2=sqrt(rx*rx+ry*ry);
    double numerator=(GRAVITATIONAL_CONSTANT)*1e6*SOLAR_MASS;
    return sqrt(numerator/r2);
}

double Simulator::signum(double val) {
	return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
}

