/*
 * Body.h
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#ifndef SRC_BODY_H_
#define SRC_BODY_H_
#include <utility>

typedef std::pair<double,double> tuple;

class Body {
	double m;
	tuple pos;
	tuple vel;

public:
	Body(double m, std::pair<double,double> pos, std::pair<double,double> vel);
	void print();
};

#endif /* SRC_BODY_H_ */
