/*
 * Body.cpp
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#include "Body.h"

Body::Body(double m, std::pair<double,double> pos, std::pair<double,double> vel) {
	this->m = m;
	this->pos =pos;
	this->vel = vel;
}

Body::~Body() {
	// TODO Auto-generated destructor stub
}

