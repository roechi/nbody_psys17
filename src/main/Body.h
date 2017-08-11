/*
 * Body.h
 *
 *  Created on: 29.04.2017
 *      Author: jaszkowic
 */

#ifndef SRC_BODY_H_
#define SRC_BODY_H_

class Body {


public:
	Body(double m, double rx, double ry, double vx, double vy);
	Body(double m, double rx, double ry, double vx, double vy, double fx, double fy);
	Body();

	double m;
	double rx;
	double ry;
	double vx;
	double vy;
	double fx = 0.0;
	double fy = 0.0;

	void update(double step);
	double distanceTo(Body b);
	void resetForce();
	void addForce(Body b, float scalingFactor, float softeningFactor);
	void print();

	virtual ~Body();
};

#endif /* SRC_BODY_H_ */
