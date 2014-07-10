/*
 * Vector.h
 *
 *  Created on: 16/02/2014
 *      Author: molina
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>
#include <iostream>

/**
 * This class contains the abstract type vector from Algebra
 */
class Vector {
private:
	float coord[4];

	////////////////////////// FUNCTIONS //////////////////////////////
public:
	Vector();
	Vector(const Vector& v);
	Vector(float x, float y, float z);
	Vector(float x, float y, float z, float norm);
	Vector(float* pointer);
	void set(float x, float y, float z);
	virtual ~Vector();
	float mod();
	Vector crossProduct(const Vector& v);
	float dotProduct(const Vector& v);
	void normalize();

	const float* getCoord() const {
		return coord;
	}

	////////////////////////// OPERATORS //////////////////////////////
public:
	float& operator[](unsigned int pos);
	Vector operator+(const Vector& v);
	Vector operator-(const Vector& v);
	Vector operator*(float f);
	Vector& operator+=(const Vector& v);
	Vector& operator=(const Vector& v);
	Vector& operator/=(float f);
	Vector matrixMultiplication(float* matrix);
};

#endif /* VECTOR_H_ */
