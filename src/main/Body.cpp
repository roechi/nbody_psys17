/*
 * Body.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "Body.h"
#include <stdio.h>
#include <math.h>
#include "Simulator.h"

Body::Body(double m, double rx, double ry, double vx, double vy) {
	this->m = m;
	this->rx = rx;
	this->ry = ry;
	this->vx = vx;
	this->vy = vy;
}

Body::Body(double m, double rx, double ry, double vx, double vy, double fx, double fy) {
	this->m = m;
	this->rx = rx;
	this->ry = ry;
	this->vx = vx;
	this->vy = vy;
	this->fx = fx;
	this->fy = fy;
}

Body::Body() {
	this->m = 0;
	this->rx = 0;
	this->ry = 0;
	this->vx = 0;
	this->vy = 0;
	this->fx = 0;
	this->fy = 0;
}

void Body::update(double step) {
	this->vx += step * this->fx / this->m;;
	this->vy += step * this->fy / this->m;
	this->rx += step * this->vx;
	this->ry += step * this->vy;
}

double Body::distanceTo(Body b){
	double dist = sqrt(pow(this->rx - b.rx,2) + pow(this->ry - b.ry, 2));
	return dist;
}

void Body::resetForce() {
	this->fx = 0.0;
	this->fy = 0.0;
}

void Body::addForce(Body b) {

    double EPS = 3E4;      // softening parameter (just to avoid infinities)
    double dx = b.rx - this->rx;
    double dy = b.ry - this->ry;
    double dist = sqrt(dx*dx + dy*dy);
    double F = (Simulator::GRAVITATIONAL_CONSTANT * this->m * b.m) / (dist*dist + EPS*EPS);
    this->fx += F * dx / dist;
    this->fy += F * dy / dist;

}


void Body::print() {
	printf("Mass: %f, Position: (%f,%f), Velocity: (%f,%f)\n", this->m, this->rx, this->ry, this->vx, this->vy);
}

