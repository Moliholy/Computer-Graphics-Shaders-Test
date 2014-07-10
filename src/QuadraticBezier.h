/*
 * QuadraticBezier.h
 *
 *  Created on: 13/03/2014
 *      Author: molina
 */

#ifndef QUADRATICBEZIER_H_
#define QUADRATICBEZIER_H_

#include "Vector.h"

/**
 * This class calculates the points of a quadratic Bézier curve
 */
class QuadraticBezier {

private:
	float t;
	Vector P[3];
public:
	void set(const Vector& P0, const Vector& P1, const Vector& P2);
	Vector getNextPoint(float increment);
	Vector getTangent();
	QuadraticBezier();
	virtual ~QuadraticBezier();
};

#endif /* QUADRATICBEZIER_H_ */
