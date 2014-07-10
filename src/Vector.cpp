/*
 * Vector.cpp
 *
 *  Created on: 16/02/2014
 *      Author: molina
 */

#include "Vector.h"

Vector::Vector() {
	coord[0] = coord[1] = coord[2] = coord[3] = 0.0f;
}

Vector::Vector(float x, float y, float z) {
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
	coord[3] = 0.0f;
}

Vector::~Vector() {
}

void Vector::set(float x, float y, float z) {
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
}

float& Vector::operator [](unsigned int pos) {
	return coord[pos];
}

float Vector::mod() {
	return sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
}

Vector Vector::crossProduct(const Vector& v) {
	float x = coord[1] * v.coord[2] - coord[2] * v.coord[1];
	float y = coord[2] * v.coord[0] - coord[0] * v.coord[2];
	float z = coord[0] * v.coord[1] - coord[1] * v.coord[0];

	return Vector(x, y, z);
}

float Vector::dotProduct(const Vector& v) {
	return coord[0] * v.coord[0] + coord[1] * v.coord[1] + coord[2] * v.coord[2];
}

Vector Vector::operator +(const Vector& v) {
	float x = coord[0] + v.coord[0];
	float y = coord[1] + v.coord[1];
	float z = coord[2] + v.coord[2];
	return Vector(x, y, z);
}

Vector Vector::operator -(const Vector& v) {
	float x = coord[0] - v.coord[0];
	float y = coord[1] - v.coord[1];
	float z = coord[2] - v.coord[2];
	return Vector(x, y, z);
}

Vector Vector::operator *(float f) {
	return Vector(coord[0] * f, coord[1] * f, coord[2] * f);
}

void Vector::normalize() {
	float m = mod();
	coord[0] /= m;
	coord[1] /= m;
	coord[2] /= m;
}

Vector::Vector(const Vector& v) {
	coord[0] = v.coord[0];
	coord[1] = v.coord[1];
	coord[2] = v.coord[2];
}

Vector& Vector::operator =(const Vector& v) {
	coord[0] = v.coord[0];
	coord[1] = v.coord[1];
	coord[2] = v.coord[2];
	return *this;
}

Vector& Vector::operator +=(const Vector& v) {
	coord[0] += v.coord[0];
	coord[1] += v.coord[1];
	coord[2] += v.coord[2];
	return *this;
}

Vector::Vector(float* pointer) {
	coord[0] = pointer[0];
	coord[1] = pointer[1];
	coord[2] = pointer[2];
	coord[3] = pointer[3];
}

Vector& Vector::operator /=(float f) {
	coord[0] /= f;
	coord[1] /= f;
	coord[2] /= f;
	return *this;
}

Vector::Vector(float x, float y, float z, float norm) {
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
	coord[3] = norm;
}

Vector Vector::matrixMultiplication(float* matrix) {
	Vector result(0, 0, 0);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i] += matrix[i * 4 + j] * coord[j];
		}
	}
	return result;
}
