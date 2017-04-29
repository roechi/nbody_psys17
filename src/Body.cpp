/*
 * Body.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "Body.h"
#include <stdio.h>

Body::Body(double m, std::pair<double,double> pos, std::pair<double,double> vel) {
	this->m = m;
	this->pos =pos;
	this->vel = vel;
}

void Body::print() {
	printf("Mass: %f, Position: (%f,%f), Velocity: (%f,%f)\n", this->m, this->pos.first, this->pos.second, this->vel.first, this->vel.second);
}

