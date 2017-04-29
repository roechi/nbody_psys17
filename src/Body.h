/*
 * Body.h
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#ifndef SRC_BODY_H_
#define SRC_BODY_H_
#include <utility>

class Body {
	double m;
	std::pair<double,double> pos;
	std::pair<double,double> vel;

public:
	Body(double m, std::pair<double,double> pos, std::pair<double,double> vel);
	virtual ~Body();
};

#endif /* SRC_BODY_H_ */
