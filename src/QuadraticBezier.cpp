/*
 * QuadraticBezier.cpp
 *
 *  Created on: 13/03/2014
 *      Author: molina
 */

#include "QuadraticBezier.h"

QuadraticBezier::QuadraticBezier() {
	t = 0.0f;
}

Vector QuadraticBezier::getNextPoint(float increment) {
	t += increment;
	if (t < 1.0f) {
		return P[0] * ((1.0f - t) * (1.0f - t)) + P[1] * (2.0f * (1.0f - t) * t)
				+ P[2] * t;
	} else {
		Vector tangent = P[2] - P[1];
		tangent.normalize();
		return tangent * (t - 1.0f) + P[2];
	}
}

void QuadraticBezier::set(const Vector& P0, const Vector& P1,
		const Vector& P2) {
	t = 0.0f;
	P[0] = P0;
	P[1] = P1;
	P[2] = P2;
}

Vector QuadraticBezier::getTangent() {
	Vector tangent = (P[1] - P[0]) * 2.0f * (1.0f - t)
			+ (P[2] - P[1]) * (2 * t);
	tangent.normalize();
	return tangent;
}

QuadraticBezier::~QuadraticBezier() {
}

