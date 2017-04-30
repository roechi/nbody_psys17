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
	    	double x = this->bodies[i].rx * 250 / 1e18;
	    	double y = this->bodies[i].ry * 250 / 1e18;
	    	printf("%f\t%f\t", x, y);
	    }
	    printf("\n");
	    addForces();
	}

}

void Simulator::generateBodies() {
    double radius = 1e18;
    double solarmass=1.98892e30;
    for (int i = 0; i < Simulator::NUM_BODIES; i++) {
      double px = 1e18*exp(-1.8)*(0.5-getRandom());
      double py = 1e18*exp(-1.8)*(0.5-getRandom());
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

      double mass = getRandom()*solarmass*10+1e20;

      this->bodies[i] = Body(mass, px,py, vx, vy);
    }

    //Put the central mass in
    this->bodies[0] = Body(1e6*solarmass, 0,0, 0,0);//put a heavy body in the center
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
      this->bodies[i].update(1e11);
    }
}

double Simulator::getRandom() {
	double r = ((double) rand() / (RAND_MAX));
	return r;
}

double Simulator::circlev(double rx, double ry) {
    double solarmass=1.98892e30;
    double r2=sqrt(rx*rx+ry*ry);
    double numerator=(6.67e-11)*1e6*solarmass;
    return sqrt(numerator/r2);
}

double Simulator::signum(double val) {
	return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
}

